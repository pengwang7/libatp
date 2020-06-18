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
