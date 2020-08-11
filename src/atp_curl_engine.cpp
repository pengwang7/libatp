/*
 * MIT License
 *
 * Copyright (c) 2019 pengwang7(https://github.com/pengwang7/libatp)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "atp_curl_engine.h"
#include "atp_thread_storage.h"

#include "glog/logging.h"

namespace atp {

/* Default user agent for http request */
#define HTTP_USERAGENT ("CURL_ENGINE(1.0)")

/* 8K */
#define DEFAULT_BUFFER_SIZE (1024 * 8)

#define pointer_safe_free(_var, _freefn) do { \
        _freefn((_var));    \
        (_var) = NULL;      \
} while (0)

struct curl_engine {
    /* Name of the CURL engine, used when explicitly requested */
    const char* name;

    /* Description of the CURL engine */
    const char* description;

    /* Init the instance */
    int (* init)(struct curl_instance* instance);

    /* Launch HTTP request */
    int (* perform)(struct curl_instance* instance, CURLcode* error);

    /* Whether or not verify peer identity */
    void (* set_ssl_verifypeer)(struct curl_instance* instance, int verifypeer);

    /* Set HTTP headers */
    void (* set_headers)(struct curl_instance* instance, struct curl_slist* headers);

    /* Set HTTP POST request url */
    void (* set_url)(struct curl_instance* instance, const char* url);

    /* Set HTTP POST request data */
    void (* set_data)(struct curl_instance* instance, const char* data, int size);

    /* Get HTTP POST response data */
    void (* get_data)(struct curl_instance* instance, char* data, int size);

    /* Destroy the instance */
    void (* destroy)(struct curl_instance** instance);
};

struct curl_instance {
    /* CURL virtual methods table */
    struct curl_engine* engine;

    CURL** curl;

    struct {
        /* The buffer for storage response data */
        char* buff;
        /* The response buffer size */
        int buff_size;
        /* The buffer current data size */
        int cursor;
    } resp_data;
};

/*
 * The CURL read callback could be call more than once,
 * So we need to append data to memory
 */
PRIVATE_API(size_t on_message(void* buff, size_t buff_size, size_t nmemb, void* args),
{
    /* Recv total data size */
    size_t total = buff_size * nmemb;

    struct curl_instance* instance = (struct curl_instance*)args;

    /* Copy the remote response data to local buffer */
    memcpy(instance->resp_data.buff + instance->resp_data.cursor, (char*)buff, total);

    /* Copy the end, need to update cursor */
    instance->resp_data.cursor += total;

    return total;

})

PRIVATE_API(int curl_tls_init(void* data),
{
    CURL** curl = (CURL**)data;
    *curl = curl_easy_init();
    if (!*curl) {
        return -1;
    }

    curl_easy_setopt(*curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(*curl, CURLOPT_TIMEOUT, 30);
    curl_easy_setopt(*curl, CURLOPT_CONNECTTIMEOUT, 5);
    curl_easy_setopt(*curl, CURLOPT_USERAGENT, HTTP_USERAGENT);

    LOG(INFO) << "curl_tls_init";

    return 0;
})

PRIVATE_API(void curl_tls_cleanup(void* data),
{
	CURL** curl = (CURL**)data;

	curl_easy_cleanup(*curl);

    pointer_safe_free(data, free);

    LOG(INFO) << "curl_tls_cleanup";

})

THREADSTORAGE_CUSTOM_SCOPE(curl_tls, curl_tls_init, curl_tls_cleanup, static);

PRIVATE_API(int core_init(struct curl_instance* instance),
{
    instance->curl = (CURL**)THREADSTORAGE_GET(&curl_tls, sizeof(*instance->curl));
    if (!instance->curl) {
        return -1;
    }

    curl_easy_setopt(*instance->curl, CURLOPT_WRITEFUNCTION, on_message);
    curl_easy_setopt(*instance->curl, CURLOPT_WRITEDATA, (void*)instance);

    instance->resp_data.cursor = 0;
    instance->resp_data.buff = (char*)calloc(1, sizeof(char) * DEFAULT_BUFFER_SIZE);
    instance->resp_data.buff_size = DEFAULT_BUFFER_SIZE;

    return 0;
})

PRIVATE_API(int core_perform(struct curl_instance* instance, CURLcode* error),
{
    instance->resp_data.cursor = 0;

    *error = curl_easy_perform(*instance->curl);
    if (*error != CURLE_OK) {
        return -1;
    }

    return 0;
})

PRIVATE_API(void core_set_ssl_verifypeer(struct curl_instance* instance, int verifypeer),
{
    curl_easy_setopt(*instance->curl, CURLOPT_SSL_VERIFYPEER, verifypeer);
})

PRIVATE_API(void core_set_headers(struct curl_instance* instance, struct curl_slist* headers),
{
    curl_easy_setopt(*instance->curl, CURLOPT_HTTPHEADER, headers);
})

PRIVATE_API(void core_set_url(struct curl_instance* instance, const char* url),
{
    curl_easy_setopt(*instance->curl, CURLOPT_URL, url);
})

PRIVATE_API(void core_set_data(struct curl_instance* instance, const char* data, int size),
{
    curl_easy_setopt(*instance->curl, CURLOPT_POSTFIELDS, data);
    curl_easy_setopt(*instance->curl, CURLOPT_POSTFIELDSIZE, size);
    curl_easy_setopt(*instance->curl, CURLOPT_POSTREDIR, CURL_REDIR_POST_ALL);
})

PRIVATE_API(void core_get_data(struct curl_instance* instance, char* data, int size),
{
    if (size < instance->resp_data.cursor) {
        return;
    }

    memset(data, 0, size);

    memcpy(data, instance->resp_data.buff, instance->resp_data.cursor);
})

PRIVATE_API(void core_destroy(struct curl_instance** instance),
{
    /*
     * We don't need to free (CURL** curl), it's a thread storage variable,
     * The pthread_key_create register cleanup function running after this thread exit.
     */
    struct curl_instance* tmp = *instance;

    pointer_safe_free(tmp->resp_data.buff, free);
    pointer_safe_free(tmp, free);

    *instance = NULL;
})

struct curl_engine post_engine_impl = {
    .name = "POST",
    .description = "curl post engine for http/https",
    .init = core_init,
    .perform = core_perform,
    .set_ssl_verifypeer = core_set_ssl_verifypeer,
    .set_headers = core_set_headers,
    .set_url = core_set_url,
    .set_data = core_set_data,
    .get_data = core_get_data,
    .destroy = core_destroy,
};

PUBLIC_API(struct curl_instance* curl_instance_new(),
{
    struct curl_instance* instance = (struct curl_instance*)calloc(1, sizeof(*instance));
    if (!instance) {
        return NULL;
    }

    instance->engine = &post_engine_impl;

    if (instance->engine->init(instance)) {
        pointer_safe_free(instance, free);
        return NULL;
    }

    return instance;
})

PUBLIC_API(int curl_instance_perform(struct curl_instance* instance, CURLcode* error),
{
    return instance->engine->perform(instance, error);
})

PUBLIC_API(void curl_instance_set_ssl_verifypeer(struct curl_instance* instance, int verifypeer),
{
    instance->engine->set_ssl_verifypeer(instance, verifypeer);
})

PUBLIC_API(void curl_instance_set_headers(struct curl_instance* instance, struct curl_slist* headers),
{
    instance->engine->set_headers(instance, headers);
})

PUBLIC_API(void curl_instance_set_request_url(struct curl_instance* instance, const char* url),
{
    instance->engine->set_url(instance, url);
})

PUBLIC_API(void curl_instance_set_post_data(struct curl_instance* instance, const char* data, int size),
{
    instance->engine->set_data(instance, data, size);
})

PUBLIC_API(void curl_instance_get_post_data(struct curl_instance* instance, char* data, int size),
{
    instance->engine->get_data(instance, data, size);
})

PUBLIC_API(void curl_instance_destroy(struct curl_instance* instance),
{
    instance->engine->destroy(&instance);
})

} /* end namespace atp */