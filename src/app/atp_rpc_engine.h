#ifndef __ATP_RPC_ENGINE_H__
#define __ATP_RPC_ENGINE_H__

namespace atp {

using SendFunctor = std::function<int32_t(int64_t handle, char* buff, uint32_t buff_len, int32_t flag)>;

using SendVFunctor = std::function<int32_t(int64_t handle, uint32_t slice_num, const char* msg_slice[], uint32_t msg_slice_len)>;


// EventHandler是RpcEngine通用事件处理的抽象，主要处理一些状态事件，与业务无关，与RpcEngine绑定使用
class EventHandler {
public:
    EventHandler() {}

    virtual ~EventHandler() {}

    // 上报服务的质量，根据请求结果与耗时来判断质量
    virtual void ReportTransportQuality(int64_t handle, int64_t time_cost_ms, int32_t ret) = 0;

    // 在请求消息处理完成时被回调
    virtual void RequestProcComplete(const std::string& name, int64_t time_cost_ms, int32_t ret) = 0;

    // 在响应消息处理完成时被回调
    virtual void ResponseProcComplete(const std::string& name, int64_t time_cost_ms, int32_t ret) = 0;

    // 添加到状态模块
    virtual void RegisterToStat(const std::string& name) = 0;

    // 从状态模块中移除
    virtual void UnregisterToStat(const std::string& name) = 0;
};


// RpcEngine处理与协议/消息，业务逻辑相关(只考虑TCP不考虑UDP)
class RpcEngine {
public:
    RpcEngine();

    virtual ~RpcEngine();

    virtual int32_t SetSendFunctor(const SendFunctor& send, const SendVFunctor& sendv);

    //virtual int32_t SetBroadcastFunctor();

    virtual int32_t SetEventHandler(EventHandler* handler);

    // 如果调用了SetSendFunctor则会使用相应的设置，否则需要子类进行override
    virtual int32_t Send(int64_t handle, char* buff, uint32_t buff_len, int32_t flag);

    // 如果调用了SetSendFunctor则会使用相应的设置，否则需要子类进行override
    virtual int32_t SendV(int64_t handle, uint32_t slice_num, const char* msg_slice[], uint32_t msg_slice_len);

    virtual void GetResourceUsed(std::unordered_map<std::string, int64_t>* resource_info) = 0;

    // RPC消息处理的入口，需要子类进行override
    virtual int32_t OnMessage(int64_t handle, const char* buff, uint32_t buff_len, uint32_t overload) = 0;

    //
    virtual int32_t Update() = 0;

protected:
    EventHandler* event_handler_;

    SendFunctor send_;

    SendVFunctor sendv_;
};

} /* end namespace atp */

#endif /* __ATP_RPC_ENGINE_H__ */
