#ifndef __ATP_RPC_SERVER_H__
#define __ATP_RPC_SERVER_H__

#include "atp_tcp_server.h"

namespace google {

namespace protobuf {

class Service;

} /* end namespace protobuf */

} /* end namespace google */

namespace atp {

class Server;

class RpcServer {
public:
    using ServerPtr = std::shared_ptr<Server>;

public:
    RpcServer(std::string ip, unsigned int port);

    ~RpcServer();

public:
    void start();

    void registerService(::google::protobuf::Service* service);

private:
    void onConnection(const ConnectionPtr& conn);

    void onMessage(const ConnectionPtr& conn, ByteBuffer& buff);

private:
    ServerPtr server_;
    ServerAddress srvaddr_;

    std::unordered_map<std::string, RpcChannelPtr> rpc_channels_;
    std::unordered_map<std::string, ::google::protobuf::Service*> services_map_;
};

} /* end namespace atp */

#endif /* __ATP_RPC_SERVER_H__ */
