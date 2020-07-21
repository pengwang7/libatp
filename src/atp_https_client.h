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

#ifndef __ATP_HTTPS_CLIENT_H__
#define __ATP_HTTPS_CLIENT_H__

#include <string>

#include <openssl/ssl.h>
#include <openssl/err.h>


struct event_base;
struct bufferevent;
struct evhttp_connection;

namespace atp {

typedef enum {
    TYPE_BINARY = 0,
    TYPE_JSON,
    TYPE_PROTOBUF,
    TYPE_NULL
} DataType;


/* A tiny https client for POST. */
class HttpsClient {
public:
    explicit HttpsClient();

    ~HttpsClient();

public:
    std::string launchRequest(std::string uri, std::string data, DataType data_type);

private:
    void doPostRequest();

    static void requestFinished(struct evhttp_request* request, void* args);

    void requestFinished(struct evhttp_request* request);

private:
    struct session {
        session(std::string remote_host, int remote_port, std::string post_uri, std::string data, DataType data_type = TYPE_JSON) {
            host = remote_host;
            port = remote_port;
            uri = post_uri;
            post_data = data;
            type = data_type;

            bev = NULL;
            base = NULL;
            conn = NULL;
        }

        ~session() {
            SSL_CTX_free(ssl_ctx);
            ssl_ctx = NULL;
        }

        SSL_CTX* ssl_ctx;

        int port;
        int method;
        int status;

        std::string uri;
        std::string host;
        std::string post_data;
        DataType type;

        std::string response;

        struct event_base* base;
        struct bufferevent* bev;
        struct evhttp_connection* conn;
    };

private:
    session* s1_;
};

} /* end namespace atp */

#endif /* __ATP_HTTPS_CLIENT_H__ */
