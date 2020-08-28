#include <google/protobuf/descriptor.h>

#include "rpc.pb.h"
#include "atp_buffer.h"
#include "atp_tcp_conn.h"
#include "atp_rpc_channel.h"
#include "glog/logging.h"

namespace atp {

RpcChannel::RpcChannel()
    : prototype_(&RpcMessage::default_instance()) {

}

RpcChannel::~RpcChannel() {
    LOG(INFO) << "RpcChannel destory";

    for (auto it = outstandings_.begin(); it != outstandings_.end(); ++ it) {
        outstanding_call out_call = it->second;
        delete out_call.response;
        delete out_call.done; 
    }
}

void RpcChannel::CallMethod(const ::google::protobuf::MethodDescriptor* method,
                    ::google::protobuf::RpcController* controller,
                    const ::google::protobuf::Message* request,
                    ::google::protobuf::Message* response,
                    ::google::protobuf::Closure* done) {
    // This function(CallMethod) for rpc client call remote method.
}

void RpcChannel::onMessage(const ConnectionPtr& conn, ByteBuffer& buff) {
    ByteBufferedReader reader(buff);

    slice ss = reader.consume(65535);
    if (ss.empty()) {
        LOG(ERROR) << "RpcChannel::OnMessage error, the message body to short";

        return;
    }

    LOG(INFO) << "The read remote data: " << ss.data();

    MessagePtr message(prototype_->New());

    RpcMessagePtr rpc_message = ::std::dynamic_pointer_cast<RpcMessage>(message);

    rpc_message->ParseFromArray(ss.data(), ss.size());

    LOG(INFO) << "RpcMessage: " << rpc_message->id() << "|" << rpc_message->service() << "|" << rpc_message->method();

    if (rpc_message) {
        onRpcMessage(conn, rpc_message);
    }
}

void RpcChannel::onRpcMessage(const ConnectionPtr& conn, const RpcMessagePtr& message) {
    switch (message->type()) {
        case REQUEST:
            onRpcRequest(conn, message);
            break;
        case RESPONSE:
            onRpcResponse(conn, message);
            break;
        default:
            LOG(ERROR) << "OnRpcMessage the message type is invalid";
    }
}

void RpcChannel::onRpcRequest(const ConnectionPtr& conn, const RpcMessagePtr& message) {
    LOG(INFO) << "OnRpcRequest ......";

    int error = RPC_SERVER_SUCCESS;
    auto iter = services_->find(message->service());
    if (iter != services_->end()) {
        ::google::protobuf::Service* service = iter->second;
        assert(service != NULL);

        const ::google::protobuf::ServiceDescriptor* descriptor = service->GetDescriptor();
        const ::google::protobuf::MethodDescriptor* method = descriptor->FindMethodByName(message->method());

        if (!method) {
            error = RPC_SERVER_NO_METHOD;
        } else {
            std::unique_ptr<::google::protobuf::Message> request(service->GetRequestPrototype(method).New());
            if (request->ParseFromString(message->request())) {
                ::google::protobuf::Message* response = service->GetResponsePrototype(method).New();
                setRpcMessageId(message->id());
                service->CallMethod(method, NULL, request.get(), response,
                    ::google::protobuf::NewCallback(this, &RpcChannel::doneCallback, conn, response));
            }
        }
    } else {
        error = RPC_SERVER_NO_SERVICE;
    }

    if (error != RPC_SERVER_SUCCESS) {

    }
}

void RpcChannel::onRpcResponse(const ConnectionPtr& conn, const RpcMessagePtr& message) {
    LOG(INFO) << "OnRpcResponse ......";
}

void RpcChannel::doneCallback(const ConnectionPtr conn, ::google::protobuf::Message* response) {
    std::unique_ptr<::google::protobuf::Message> body(response);
    RpcMessage message;
    message.set_type(RESPONSE);
    message.set_id(id_);
    message.set_response(body->SerializeAsString());

    std::string buff;
    message.SerializeToString(&buff);
    conn->send(buff.c_str(), buff.length());
}

} /* end namespace atp */
