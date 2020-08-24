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

#ifndef __ATP_CURL_ENGINE_H__
#define __ATP_CURL_ENGINE_H__

#include <curl/curl.h>

#include "app/atp_api.h"

namespace atp {

struct curl_instance;

DEFINE_PUBLIC_API(struct curl_instance* curl_instance_new())

DEFINE_PUBLIC_API(int curl_instance_perform(struct curl_instance* instance, CURLcode* error))

DEFINE_PUBLIC_API(void curl_instance_set_ssl_verifypeer(struct curl_instance* instance, int verifypeer))

DEFINE_PUBLIC_API(void curl_instance_set_headers(struct curl_instance* instance, struct curl_slist* headers))

DEFINE_PUBLIC_API(void curl_instance_set_request_url(struct curl_instance* instance, const char* url))

DEFINE_PUBLIC_API(void curl_instance_set_post_data(struct curl_instance* instance, const char* data, int size))

DEFINE_PUBLIC_API(void curl_instance_get_post_data(struct curl_instance* instance, char* data, int size))

DEFINE_PUBLIC_API(void curl_instance_destroy(struct curl_instance* instance))

} /* end namespace atp */

#endif /* __ATP_CURL_ENGINE_H__ */
