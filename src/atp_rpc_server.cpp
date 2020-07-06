#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>

#include "atp_rpc_channel.h"
#include "atp_rpc_server.h"
#include "glog/logging.h"

namespace atp {

RpcServer::RpcServer(std::string ip, unsigned int port) {
    srvaddr_.addr_ = ip;
    srvaddr_.port_ = port;

    int threads = getSystemCPUProcessers();

    ServerAddress srvaddr = {
        .addr_ = ip,
        .port_ = port,
    };
        
    server_.reset(new Server("RpcServer", srvaddr, 6));
    server_->setConnectionCallback(std::bind(&RpcServer::onConnection, this, std::placeholders::_1));
    server_->setMessageCallback(std::bind(&RpcServer::onMessage, this, std::placeholders::_1, std::placeholders::_2));
}

RpcServer::~RpcServer() {

}

void RpcServer::registerService(google::protobuf::Service* service) {
    const google::protobuf::ServiceDescriptor* descriptor = service->GetDescriptor();

    if (services_map_.insert({descriptor->full_name(), service}).second == false) {
        LOG(ERROR) << "RpcServer register service error: " << descriptor->full_name();
    }

    LOG(INFO) << "RpcServer register servce: " << descriptor->full_name();
}

void RpcServer::onConnection(const ConnectionPtr& conn) {
    LOG(INFO) << "============RpcServer::OnConnection===============";
}

void RpcServer::onMessage(const ConnectionPtr& conn, ByteBuffer& buff) {
    RpcChannelPtr channel(new RpcChannel());
    channel->setRpcServices(&services_map_);
    channel->onMessage(conn, buff);
}

void RpcServer::start() {
    server_->start();
}

} /* end namespace atp */
