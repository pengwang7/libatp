#ifndef __ATP_RPC_CHANNEL_H__
#define __ATP_RPC_CHANNEL_H__

#include <map>
#include <google/protobuf/service.h>

#include "atp_cbs.h"


namespace google {

namespace protobuf {

class Descriptor;
class ServiceDescriptor;
class MethodDescriptor;

class Closure;

class RpcController;

class Message;
class Service;

} /* end namespace protobuf */

} /* end namespace google */

namespace atp {

typedef enum {
    RPC_SERVER_ERROR_BASE   =   (-700),
    RPC_SERVER_SUCCESS      =   (0),
    RPC_SERVER_NO_SERVICE   =   (RPC_SERVER_ERROR_BASE - 1),
    RPC_SERVER_NO_METHOD    =   (RPC_SERVER_ERROR_BASE - 2)
} RpcServerError;


typedef std::unordered_map<std::string, ::google::protobuf::Service*> ProtobufServicesMap;

class RpcChannel : public ::google::protobuf::RpcChannel {
public:
    RpcChannel();

    virtual ~RpcChannel();

public:
    void CallMethod(const ::google::protobuf::MethodDescriptor* method,
                        ::google::protobuf::RpcController* controller,
                        const ::google::protobuf::Message* request,
                        ::google::protobuf::Message* response,
                        ::google::protobuf::Closure* done);

    void setRpcServices(const ProtobufServicesMap* services) {
        services_ = services;
    }

    void onMessage(const ConnectionPtr& conn, ByteBuffer& buff);

private:
    void onRpcMessage(const ConnectionPtr& conn, const RpcMessagePtr& message);

    void onRpcRequest(const ConnectionPtr& conn, const RpcMessagePtr& message);

    void onRpcResponse(const ConnectionPtr& conn, const RpcMessagePtr& message);

    void doneCallback(::google::protobuf::Message* response, int64_t id);

private:
    typedef struct {
        ::google::protobuf::Message* response;
        ::google::protobuf::Closure* done;        
    } outstanding_call;

    const ::google::protobuf::Message* prototype_;

    std::map<int64_t, outstanding_call> outstandings_;

    const ProtobufServicesMap* services_;
};

} /* end namespace atp */

#endif /* __ATP_RPC_CHANNEL_H__ */
