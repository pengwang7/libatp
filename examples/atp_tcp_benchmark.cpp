#include <memory>
#include "net/atp_config.h"
#include "net/atp_tcp_server.h"
#include "app/atp_any.hpp"
#include "app/atp_thread_storage.hpp"

///////////////////////////////////

#include "app/atp_base64.h"
#include "app/atp_curl_engine.h"
#include "app/atp_event_https.h"
///////////////////////////////////
using namespace atp;

#ifndef TEST_1
    #define TEST_1
#endif

//THREADSTORAGE_CUSTOM_SCOPE(weak_conn_ctx, NULL, NULL, );

void atp_logger_init() {
    FLAGS_alsologtostderr = true;                                                
    FLAGS_colorlogtostderr = true;
    FLAGS_logbufsecs = 0;                                                        
    FLAGS_max_log_size = 1800;                                                   

    google::InitGoogleLogging("test");                                           
    google::SetLogDestination(google::GLOG_INFO,"log-");                         
}

class Resp200Server {
public:
    Resp200Server() {
        ServerAddress srvaddr = {
            .addr_ = std::string("0.0.0.0"),
            .port_ = 7788,
        };
            
        server_.reset(new Server("resp-200-server", srvaddr, 4));
        server_->setConnectionCallback(std::bind(&Resp200Server::onConnection, this, std::placeholders::_1));
        server_->setMessageCallback(std::bind(&Resp200Server::onMessage, this, std::placeholders::_1, std::placeholders::_2));
    }

    ~Resp200Server() {
        LOG(INFO) << "Resp200Server destroy.";
    }

    void start() {
        server_->start();
    }
    
private:
    void onConnection(const ConnectionPtr& conn) {
        if (ATP_NET_DEBUG_ON) {
            LOG(INFO) << "Resp200Server conn uuid: " << conn->getUUID();
        }

        LOG(INFO) << "on connection conn1 use count: " << conn.use_count();

        EntryPtr entry(new Entry(conn));

        LOG(INFO) << "on connection entry use count: " << entry.use_count();

        server_->timingWheelInsert(entry);

        LOG(INFO) << "on connection entry2 use count: " << entry.use_count();

        LOG(INFO) << "on connection conn2 use count: " << conn.use_count();

        WeakEntryPtr weak_entry(entry);
        conn->setContext(weak_entry);
    }

    void onMessage(const ConnectionPtr& conn, ByteBuffer& read_buf) {
        ByteBufferedReader io_reader(read_buf);
        if (ATP_NET_DEBUG_ON) {
            slice ss = io_reader.consume(65535);
            LOG(INFO) << "Resp200Server conn read data: " << ss.size() << ":" << ss.toString();
        }

        LOG(INFO) << "on message conn1 use count: " << conn.use_count();
        assert(!conn->getContext().empty());
        WeakEntryPtr weak_entry(any_cast<WeakEntryPtr>(conn->getContext()));
        EntryPtr entry(weak_entry.lock());

        LOG(INFO) << "on message entry use1 count: " << entry.use_count();
        if (entry) {
            LOG(INFO) << "on message push entry.";
            server_->timingWheelInsert(entry);
        }

        LOG(INFO) << "on message entry use2 count: " << entry.use_count();

        LOG(INFO) << "on message conn2 use count: " << conn.use_count();

        std::string resp_200_message = "";
        resp_200_message += "HTTP/1.1 200 OK\r\n";
        resp_200_message += "Server: Resp200Server\r\n";
        resp_200_message += "Content-Type: text/html\r\n";
        resp_200_message += "Content-Length: 10\r\n\r\n";
        resp_200_message += "aaabbbcccdea";
        resp_200_message += "\r\n";

        LOG(INFO) << "response message len: " << resp_200_message.length();
        conn->send(resp_200_message.c_str(), resp_200_message.length());
        //conn->close();
    }

private:
    std::unique_ptr<Server> server_;
    ServerAddress srvaddr_;
};

void test1() {
    // Resp200Server test
    std::unique_ptr<Resp200Server> resp_200_server;
    resp_200_server.reset(new Resp200Server());
    resp_200_server->start();
}

void test2() {

}

int main() {
#ifdef TEST_1
    test1();
#endif

#ifdef TEST_2

#endif
    return 0;
}
