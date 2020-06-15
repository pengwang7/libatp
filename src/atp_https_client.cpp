#include "libevent/event2/http.h"
#include "libevent/event2/event.h"
#include "libevent/event2/buffer.h"
#include "libevent/event2/bufferevent.h"
#include "libevent/event2/bufferevent_ssl.h"

#include "atp_https_client.h"

namespace atp {

HttpsClient::HttpsClient() {
}

HttpsClient::~HttpsClient() {

}    

std::string HttpsClient::launchRequest(std::string uri, std::string data, DataType data_type) {
    std::string result = "";

    struct evhttp_uri* http_uri = evhttp_uri_parse(uri.c_str());
    if (!http_uri) {
        return result;
    }

    const char* host = evhttp_uri_get_host(http_uri);
    int port = evhttp_uri_get_port(http_uri);
    if (port == -1) {
        port = 443;
    }

    const char* path = evhttp_uri_get_path(http_uri);
    if (!path) {
        return result;
    }

    const char* query = evhttp_uri_get_query(http_uri);
    if (query == NULL) {
        uri = path;
    } else {
        uri = std::string(path) + std::string("?") + std::string(query);
    }

    s1_ = new session(std::string(host), port, uri, data, data_type);
    if (!s1_) {
        return result;
    }

#if (OPENSSL_VERSION_NUMBER < 0x10100000L) || \
        (defined(LIBRESSL_VERSION_NUMBER) && LIBRESSL_VERSION_NUMBER < 0x20700000L)
    // Initialize OpenSSL
    SSL_library_init();
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();
#endif

    s1_->method = EVHTTP_REQ_POST;
    s1_->base = event_base_new();
    s1_->ssl_ctx = SSL_CTX_new (SSLv23_client_method());
    if (!s1_->ssl_ctx) {
        event_base_free(s1_->base);
        delete s1_;

        return result;
    }

    // One-Way authentication for https.
    // SSL_CTX_load_verify_locations(s1->ssl_ctx, "/cfg/etc/gdms/certificate", NULL);
    SSL_CTX_set_verify (s1_->ssl_ctx, SSL_VERIFY_NONE, NULL);
    // SSL_CTX_set_cert_verify_callback(s1->ssl_ctx, cert_verify_callback, NULL);

    doPostRequest();

    event_base_loop(s1_->base, 0);

    if (s1_->status != HTTP_OK) {
        result = "";
    } else {
        result = std::string(s1_->response);
    }

    evhttp_connection_free(s1_->conn);
    event_base_free(s1_->base);

    delete s1_;

    return result;
}

void HttpsClient::doPostRequest() {

}

void HttpsClient::requestFinished(struct evhttp_request* request, void* args) {

}

void HttpsClient::requestFinished(struct evhttp_request* request) {

}

} /* end namespace atp */
