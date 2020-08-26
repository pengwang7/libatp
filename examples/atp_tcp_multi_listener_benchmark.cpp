#include <vector>

#include "glog/logging.h"

#include "app/atp_https_client.h"
//#include "app/atp_send_helper.hpp"
#include "net/atp_tcp_server.h"
#include "net/atp_event_loop.h"
#include "net/atp_event_loop_thread_pool.h"

using namespace atp;

#ifndef TEST_1
	#define TEST_1
#endif

void atp_logger_init() {
    FLAGS_alsologtostderr = true;                                                
    FLAGS_colorlogtostderr = true;
    FLAGS_logbufsecs = 0;                                                        
    FLAGS_max_log_size = 1800;                                                   
                                                                                 
    google::InitGoogleLogging("test");                                           
    google::SetLogDestination(google::GLOG_INFO,"log-");                         
}

using ServerPtr = std::shared_ptr<Server>;

class EchoServer {
public:
	EchoServer(std::string ip, unsigned int port, int listeners) {
		srvaddr_.addr_ = ip;
		srvaddr_.port_ = port;

        hold_event_loop_.reset(new EventLoop());
        event_loop_pool_.reset(new EventLoopPool(listeners));
        event_loop_pool_->autoStart();

		for (int i = 0; i < listeners; ++ i) {
			LOG(INFO) << "The " << i + 1 << " listener create";

            EventLoop* event_loop = event_loop_pool_->getIOEventLoop();

			ServerPtr server;
			server.reset(new Server("multi-listeners-echo-server", event_loop, srvaddr_, 0));
			server->setConnectionCallback(std::bind(&EchoServer::onConnection, this, std::placeholders::_1));
			server->setMessageCallback(std::bind(&EchoServer::onMessage, this, std::placeholders::_1, std::placeholders::_2));
            server->start();
			servers_.push_back(server);
		}
	}

	~EchoServer() {

	}

	void start() {
	    // The hold_event_loop_ internal had event_watcher events.
	    hold_event_loop_->dispatch();
	}

	void stop() {
		servers_.clear();
	}

private:
	void onConnection(const ConnectionPtr& conn) {
		LOG(INFO) << "conn uuid: " << conn->getUUID();
	}

void onMessage(const ConnectionPtr& conn, ByteBuffer& read_buf) {
	ByteBufferReader io_reader(read_buf);
	LOG(INFO) << "conn read data: " << io_reader.consume(65535).toString();

	std::string resp_message = "pong-pong-pong-pong-pong-pong-pong-pong-pong-pong-pong-pong\r\n";
	conn->send(resp_message.c_str(), resp_message.length());
	conn->close();
}

private:
    std::unique_ptr<EventLoop> hold_event_loop_;
    std::unique_ptr<EventLoopPool> event_loop_pool_;
	std::vector<ServerPtr> servers_;
	ServerAddress srvaddr_;
};

void test1() {
	int listeners = getSystemCPUProcessers();
	EchoServer server(std::string("0.0.0.0"), 8012, listeners);
	server.start();
}

int main() {
    atp_logger_init();
#ifdef TEST_1
	test1();
#endif
	return 0;
}


