#ifndef __ATP_RPC_H__
#define __ATP_RPC_H__

#include "rpc_cbs.h"
#include "rpc_engine.h"

namespace atp {

// RPC错误码定义
typedef enum {
    RPC_ERROR_BASE              = (-9000),
    RPC_INVALID_PARAM           = RPC_ERROR_BASE - 1,   // 参数错误
    RPC_ENCODE_FAILED           = RPC_ERROR_BASE - 2,   // 编码错误
    RPC_DECODE_FAILED           = RPC_ERROR_BASE - 3,   // 解码错误
    RPC_RECV_EXCEPTION_MSG      = RPC_ERROR_BASE - 4,   // 收到EXCEPTION消息
    RPC_UNKNOWN_TYPE            = RPC_ERROR_BASE - 5,   // 消息类型错误
    RPC_UNSUPPORT_FUNCTION_NAME = RPC_ERROR_BASE - 6,   // 不支持的函数名
    RPC_SESSION_NOT_FOUND       = RPC_ERROR_BASE - 7,   // session已过期
    RPC_SEND_FAILED             = RPC_ERROR_BASE - 8,   // 发送失败
    RPC_REQUEST_TIMEOUT         = RPC_ERROR_BASE - 9,   // 请求超时
    RPC_FUNCTION_NAME_EXISTED   = RPC_ERROR_BASE - 10,  // 服务名已存在
    RPC_SYSTEM_ERROR            = RPC_ERROR_BASE - 11,  // 系统错误
    RPC_PROCESS_TIMEOUT         = RPC_ERROR_BASE - 13,  // 服务处理超时
    PRC_BROADCAST_FAILED        = RPC_ERROR_BASE - 14,  // 广播失败
    RPC_FUNCTION_NAME_UNEXISTED = RPC_ERROR_BASE - 15,  // 服务名不存在
    RPC_PEBBLE_RPC_ERROR_BASE   = RPC_ERROR_BASE - 100, // PEBBE RPC错误码BASE
    RPC_RPC_UTIL_ERROR_BASE     = RPC_ERROR_BASE - 200, // RPC辅助工具错误码BASE
    RPC_SYSTEM_OVERLOAD_BASE    = RPC_ERROR_BASE - 300, // 系统过载BASE
    RPC_MESSAGE_EXPIRED         = RPC_SYSTEM_OVERLOAD_BASE - 1, // 系统过载-消息过期
    RPC_TASK_OVERLOAD           = RPC_SYSTEM_OVERLOAD_BASE - 2, // 系统过载-并发任务过载
    RPC_SUCCESS                 = 0,
} RpcErrorCode;

typedef enum {
    RPC_CALL      = 1,
    RPC_REPLY     = 2,
    RPC_EXCEPTION = 3,
    RPC_ONEWAY    = 4,
} RpcMessageType;

typedef enum {
    VERSION_0 = 0,
} RpcVersion;

struct RpcHeader {
    RpcHeader() {
        version = VERSION_0;
        message_type = RPC_EXCEPTION;
        session_id = 0;
        arrived_ms = -1;
        func_name = "";
        dest = NULL;
    }

    RpcHeader(const RpcHeader& rhs) {
        version = rhs.version;
        message_type = rhs.message_type;
        session_id = rhs.session_id;
        arrived_ms = rhs.arrived_ms;
        func_name = rhs.func_name;
        dest = rhs.dest;
    }

    int32_t version;        // PRC版本号
    int32_t message_type;   // 消息类型
    uint64_t session_id;    // session id
    int64_t arrived_ms;     // 消息到达时间
    std::string func_name;  // 调用函数名称

    RpcEngine* dest;
};

struct RpcException {
    RpcException() {
        code = 0;
        message = std::string("");
    }

    int32_t code;
    std::string message;
};

struct RpcSession;
class CycleTimer;

class Rpc : public RpcEngine {
public:
    Rpc();

    virtual ~Rpc();

public:
    // 消息处理函数，对消息进行解码
    int32_t OnMessage(int64_t handle, const char* buff, uint32_t buff_len, uint32_t overload) override;

    // 获取RPC使用的资源情况，timer的数量以及RpcSession
    void GetResourceUsed(std::unordered_map<std::string, int64_t>* resource_info) override;

    int32_t Update() override;

public:
    static const uint32_t REQ_PROC_TIMEOUT_MS = 20 * 1000; // 20s

    // 应用层无需关注
    uint64_t GenSessionId() {
        return session_id_ ++;
    }

    void SetProcRequestTimeoutMS(uint32_t proc_req_timeout_ms) {
        proc_req_timeout_ms_ = proc_req_timeout_ms;
    }

protected:
    virtual int32_t HeaderEncode(const RpcHeader& header, char* buff, uint32_t buff_len) = 0;

    virtual int32_t HeaderDecode(const char* buff, uint32_t buff_len, RpcHeader* header) = 0;

    virtual int32_t ExceptionEncode(const RpcException& exception, char* buff, uint32_t buff_len) = 0;

    virtual int32_t ExceptionDecode(const char* buff, uint32_t buff_len, RpcException* exception) = 0;

    virtual int32_t ProcessRequest(int64_t handle, const RpcHeader& header, const char* buff, uint32_t buff_len);

private:
    int32_t SendResponse(uint64_t session_id, int32_t ret, const char* buff, uint32_t buff_len);
    
    int32_t SendMessage(int64_t handle, const RpcHeader& header, const char* buff, uint32_t buff_len);
    
    int32_t OnTimeout(uint64_t session_id);

    int32_t ProcessResponse(const RpcHeader& header, const char* buff, uint32_t buff_len);
    
    int32_t ResponseException(int64_t handle, int32_t ret, const RpcHeader& header, const char* buff = NULL, uint32_t buff_len = 0);
    
    int32_t ProcessRequestImp(int64_t handle, const RpcHeader& header, const char* buff, uint32_t buff_len);

    // 內部服务质量反馈
    inline void ReportTransportQuality(int64_t handle, int32_t ret_code, int64_t time_cost_ms);
    
    inline void RequestProcComplete(const std::string& name, int32_t result, int32_t time_cost_ms);
    
    inline void ResponseProcComplete(const std::string& name, int32_t result, int32_t time_cost_ms);

private:
    std::unordered_map<std::string, OnRpcRequest> service_map_;
    std::unordered_map<uint64_t, std::shared_ptr<RpcSession>> session_map_;

    std::unique_ptr<CycleTimer> tiemr_;
    //CycleTimer* timer_;
    char rpc_header_buff[1024];
    char rpc_exception_buff[4096];

    uint32_t proc_req_timeout_ms_;
    uint64_t session_id_;    
};

} /* end namespace atp */

#endif /* __ATP_RPC_H__ */
