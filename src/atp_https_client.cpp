#include "libevent/event2/http.h"
#include "libevent/event2/event.h"
#include "libevent/event2/buffer.h"
#include "libevent/event2/bufferevent.h"
#include "libevent/event2/bufferevent_ssl.h"

#include "glog/logging.h"

#include "atp_https_client.h"

namespace atp {


HttpsClient::HttpsClient() {
    s1_ = NULL;
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
    SSL* ssl = SSL_new(s1_->ssl_ctx);
    s1_->bev = bufferevent_openssl_socket_new(s1_->base, -1, ssl,
        BUFFEREVENT_SSL_CONNECTING,
        0 | BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS);
    if (!s1_->bev) {
        LOG(ERROR) << "The bufferevent is nil";

        return;
    }

    bufferevent_openssl_set_allow_dirty_shutdown(s1_->bev, 1);

    struct evhttp_request* request = evhttp_request_new(HttpsClient::requestFinished, this);
    struct evkeyvalq* headers = evhttp_request_get_output_headers(request);
    evhttp_add_header(headers, "Host", s1_->host.c_str());
    evhttp_add_header(headers, "Connection", "close");

    if (s1_->type == TYPE_JSON) {
        /* Set content-type */
        evhttp_add_header(headers, "Content-Type", "application/json;charset=UTF-8");
    }
    
    s1_->conn = evhttp_connection_base_bufferevent_new(s1_->base, NULL, s1_->bev, s1_->host.c_str(), s1_->port);

    /* Set connect timeout */
    evhttp_connection_set_timeout(s1_->conn, 10);

    /* Set post data */
    evbuffer_add(evhttp_request_get_output_buffer(request), s1_->post_data.c_str(), s1_->post_data.size());

    /* Send https post request */
    evhttp_make_request(s1_->conn, request, EVHTTP_REQ_POST, s1_->uri.c_str());
}

void HttpsClient::requestFinished(struct evhttp_request* request, void* args) {
    HttpsClient* client = static_cast<HttpsClient*>(args);
    if (client) {
        client->requestFinished(request);
    }
}

void HttpsClient::requestFinished(struct evhttp_request* request) {
    const int status_code = request ? evhttp_request_get_response_code(request) : 0;
    if (status_code != HTTP_OK) {
        LOG(INFO) << "statu_code error: " << status_code;
    }

    s1_->status = status_code;

    char buffer[256] = {0};
    unsigned long oslerr;

    int printed_err = 0;
    int errcode = EVUTIL_SOCKET_ERROR();

    /* Print out the OpenSSL error queue that libevent */
    /* squirreled away for us, if any. */
    while ((oslerr = bufferevent_get_openssl_error(s1_->bev))) {
        ERR_error_string_n(oslerr, buffer, sizeof(buffer));
        printed_err = 1;
    }

    /* If the OpenSSL error queue was empty, maybe it was a */
    /* socket error, let's try printing that. */
    if (!printed_err && s1_->status != HTTP_OK) {
        LOG(INFO) << "The socket error = " << evutil_socket_error_to_string(errcode);
    }

    /* Get remote response data */
    if (request) {
        struct evbuffer* buff = evhttp_request_get_input_buffer(request);
        evbuffer_add (buff, "", 1);
        char* payload = reinterpret_cast<char*>(evbuffer_pullup(buff, -1));
        s1_->response = std::string(payload);
    }
}

} /* end namespace atp */
