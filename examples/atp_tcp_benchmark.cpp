#include "net/atp_config.h"
#include "net/atp_tcp_server.h"

///////////////////////////////////

#include "app/atp_base64.h"
#include "app/atp_curl_engine.h"
#include "app/atp_event_https.h"
///////////////////////////////////
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
    }

	void onMessage(const ConnectionPtr& conn, ByteBuffer& read_buf) {
		ByteBufferReader io_reader(read_buf);
        if (ATP_NET_DEBUG_ON) {
            slice ss = io_reader.consume(65535);
            LOG(INFO) << "Resp200Server conn read data: " << ss.size() << ":" << ss.toString();
        }
	
		std::string resp_200_message = "";
		resp_200_message += "HTTP/1.1 200 OK\r\n";
		resp_200_message += "Server: Resp200Server\r\n";
		resp_200_message += "Content-Type: text/html\r\n";
		resp_200_message += "Content-Length: 10\r\n\r\n";
		resp_200_message += "aaabbbcccde";
		resp_200_message += "\r\n";

        LOG(INFO) << "response message len: " << resp_200_message.length();
		conn->send(resp_200_message.c_str(), resp_200_message.length());
		conn->close();
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

