#ifndef __ATP_REDIS_CLIENT_H__
#define __ATP_REDIS_CLIENT_H__

namespace atp {
/* The hiredis interface(sync) for C++ */

typedef struct {
    std::string addr_;
    unsigned int port_;
    std::string passwd_;
} RedisServerInfo;

class RedisClientImpl {
public:
    explicit RedisClientImpl(RedisServerInfo server_info);
    ~RedisClientImpl();

public:
    void redisConnect();
    void redisHeartBeat();
    void redisLibeventAttach();
    void redisAsyncSetConnectCallback();
    void redisAsyncSetDisconnectionCallback();
    void redisExecuteCommandCallback();
};

} /* end namespace atp */
#endif /* __ATP_REDIS_CLIENT_H__ */
