#ifndef __ATP_CURL_ENGINE_H__
#define __ATP_CURL_ENGINE_H__

#include <curl/curl.h>

#include "atp_api.h"

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
