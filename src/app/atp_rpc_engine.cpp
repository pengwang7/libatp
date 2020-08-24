#include "rpc_engine.h"

namespace atp {

RpcEngine::RpcEngine() {
    event_handler_ = NULL;
}

RpcEngine::~RpcEngine() {

}

int32_t RpcEngine::SetSendFunctor(const SendFunctor& send, const SendVFunctor& sendv) {
    send_ = send;
    sendv_ = send;

    return 0;
}

int32_t RpcEngine::SetEventHandler(EventHandler* handler) {
    event_handler_ = handler;

    return 0;
}

int32_t RpcEngine::Send(int64_t handle, char* buff, uint32_t buff_len, int32_t flag) {
    if (send_) {
        return send_(handle, buff, buff_len, flag);
    }

    return -1;
}

int32_t RpcEngine::SendV(int64_t handle, uint32_t slice_num, const char* msg_slice[], uint32_t msg_slice_len) {
    if (sendv_) {
        return sendv_(handle, slice_num, msg_slice, msg_slice_len);
    }

    return -1;
}

} /* end namespace atp */
