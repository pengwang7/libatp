#include <vector>

#include "glog/logging.h"

#include "atp_tcp_server.h"

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

		for (int i = 0; i < listeners; ++ i) {
			LOG(INFO) << "The " << i + 1 << " listener create";
			
			ServerPtr server;
			server.reset(new Server("multi-listeners-echo-server", srvaddr_, 0));
			server->setConnectionCallback(std::bind(&EchoServer::onConnection, this, std::placeholders::_1));
			server->setMessageCallback(std::bind(&EchoServer::onMessage, this, std::placeholders::_1, std::placeholders::_2));

			servers_.push_back(server);
		}
	}

	~EchoServer() {

	}

	void start() {
		for (auto iter = servers_.begin(); iter != servers_.end(); iter ++) {
			(*iter)->start();
		}
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
	std::vector<ServerPtr> servers_;
	ServerAddress srvaddr_;
};

void test1() {
	int listeners = getSystemCPUProcessers() * 2;
	EchoServer server(std::string("0.0.0.0"), 7788, listeners);
	server.start();
}

int main() {
#ifdef TEST_1
	test1();
#endif
	return 0;
}


