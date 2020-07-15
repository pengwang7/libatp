#include "echo_server.pb.h"
#include "atp_rpc_server.h"
#include "atp_rsa_crypto.hpp"
#include "glog/logging.h"

using namespace atp;

class EchoServiceImpl : public EchoService {
public:
    EchoServiceImpl() {};

    virtual ~EchoServiceImpl() {};

    virtual void Echo(::google::protobuf::RpcController* controller,
                        const ::atp::EchoRequest* request,
                        ::atp::EchoResponse* response,
                        ::google::protobuf::Closure* done) {

        LOG(INFO) << "EchoServiceImpl request: " << request->message();

        response->set_message("wwwwwwwwwwwwww");

        done->Run();
    }
};
                   
static void atp_logger_init() {
    FLAGS_alsologtostderr = true;
    FLAGS_colorlogtostderr = true;
    FLAGS_logbufsecs = 0;
    FLAGS_max_log_size = 1800;

    google::InitGoogleLogging("test");
    google::SetLogDestination(google::GLOG_INFO,"log-");
}

static void atp_logger_close() {
    google::ShutdownGoogleLogging();
}

int main() {
    atp_logger_init();
 
    EchoServiceImpl echo_service;
    RpcServer server("127.0.0.1", 7765);
    server.registerService(&echo_service);
    server.start();

//    ::google::protobuf::ShutdownProtobufLibrary();
    atp_logger_close();

    return 0;
}
