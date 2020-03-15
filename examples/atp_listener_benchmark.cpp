#include "glog/logging.h"

#include "atp_uuid.h"
#include "atp_listener.h"
#include "atp_event_loop.h"
#include "atp_event_loop_thread_pool.h"
#include "atp_tcp_server.h"

using namespace atp;

void atp_logger_init() {
    FLAGS_alsologtostderr = true;                                                
    FLAGS_colorlogtostderr = true;
    FLAGS_logbufsecs = 0;                                                        
    FLAGS_max_log_size = 1800;                                                   
                                                                                 
    google::InitGoogleLogging("test");                                           
    google::SetLogDestination(google::GLOG_INFO,"log-");                         
}

void atp_logger_close() {
    google::ShutdownGoogleLogging();
}

/* The new_conn_handle change fd to conn */
static void new_conn_handle(int fd, const std::string& taddr, void* args) {
    LOG(INFO) << "fd = " << fd << "  remote address = " << taddr;
}

class tt {
public:
    tt() {
        LOG(INFO) << "wangpeng";
    }
};
class EchoServer {
public:
    EchoServer() {
        srvaddr_.addr_ = "0.0.0.0";
        srvaddr_.port_ = 7788;
        /*
        for (int i = 0; i < 4; ++ i) {

        }
        */

        server_.reset(new Server("echo-server", srvaddr_, 0));

        server_->setConnectionCallback(std::bind(&EchoServer::onConnection, this, std::placeholders::_1));
        server_->setMessageCallback(std::bind(&EchoServer::onMessage, this, std::placeholders::_1, std::placeholders::_2));

        LOG(INFO) <<"EchoServer Init.";
    }

    ~EchoServer() {
        LOG(INFO) << "EchoServer destroy.";
    }

public:
    void start() {
        server_->start();
    }
    
private:
    void onConnection(const ConnectionPtr& conn) {
        LOG(INFO) << "on connection.";

        LOG(INFO) << "remote address: " << conn->getAddress();
    }

    void onMessage(const ConnectionPtr& conn, ByteBuffer& buffer) {
        LOG(INFO) << "on message.";

        ByteBufferReader reader(buffer);
        LOG(INFO) << "data: " << reader.consume(8192).data();
    }
/*
    void onClose(const ConnectionPtr& conn) {
        LOG(INFO) << "on close connection.";
    }
*/
private:
    std::unique_ptr<Server> server_;
    ServerAddress srvaddr_;
};

int main() {
    std::unique_ptr<EchoServer> echo_server;
    echo_server.reset(new EchoServer());
    echo_server->start();
    
    return 0;
}

int main03() {
    atp_logger_init();
    LOG(INFO) << "uuid test";

    UUIDGenerator generator;

    LOG(INFO) << generator.generateUUID();
    LOG(INFO) << generator.generateUUID().length();
    atp_logger_close();
    
    return 0;
}

int main02() {
    atp_logger_init();
    {
        const int thd_size = 1;
        EventLoopPool* pool = new EventLoopPool(thd_size);
        assert(pool);

        LOG(INFO) << "main: " << std::this_thread::get_id();
        assert(pool->autoStart());
        assert(pool->autoStop());
        
        LOG(INFO) << "event loop pool stop, wait join thread ...";

        pool->autoJoin();

        delete pool;
    } 
    atp_logger_close();

    return 0;
}

int main01() {
    atp_logger_init();

    EventLoop* event_loop = new EventLoop();
    assert(event_loop);
    
    Listener* listener = new Listener(event_loop, std::string("0.0.0.0"), 5060);
    assert(listener);
    
    listener->setNewConnCallback(&new_conn_handle);
    listener->listen();
    listener->accept();
    event_loop->dispatch();

    delete event_loop;
    event_loop = nullptr;
    
    atp_logger_close();
    
    return 0;
}
