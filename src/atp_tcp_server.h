#ifndef __SERVICE_H__
#define __SERVICE_H__

#include <sstream>

#include "jsoncpp/json.h"

#include "atp_cbs.h"

namespace atp {

class Listener;
class EventLoop;
class EventLoopPool;
class DynamicThreadPool;
class UUIDGenerator;

typedef struct {
    std::string addr_;
    unsigned int port_;
} ServerAddress;

class Codec {
public:
    explicit Codec() {}
    ~Codec() {}

public:
    std::string encode(const Json::Value& table) {
        std::string res("");
        if (table == Json::Value::null) {
            return res;
        }

        std::ostringstream out;
        Json::StreamWriterBuilder streamBuilder;
        std::unique_ptr<Json::StreamWriter> streamWriter(streamBuilder.newStreamWriter());
        streamWriter->write(table, &out);
        res = out.str();

        return res;
    }

    int decode(const std::string str, Json::Value& table) {
        JSONCPP_STRING errs;
        Json::CharReaderBuilder charBuilder;
        std::unique_ptr<Json::CharReader> charReader(charBuilder.newCharReader());
        int res = charReader->parse(str.c_str(), str.c_str() + str.length(), &table, &errs);
        if (!res || !errs.empty()) {
            return -1;
        }
        
        return 0;
    }
};


class Server      : public STATE_MACHINE_INTERFACE {
public:
    explicit Server(std::string name, ServerAddress server_address, int thread_nums);
    ~Server();

public:
    void start();
    void stop();

public:
    void setConnectionCallback(const ConnectionCallback& fn) {
        conn_fn_ = fn;        
    }

    void setMessageCallback(const ReadMessageCallback& fn) {
        message_fn_ = fn;
    }

private:
    int getSystemCPUProcessers();
    void startEventLoopPool();
    void stopEventLoopPool();
    void handleNewConnection(int fd, const std::string& taddr, void* args);
    EventLoop* getIOEventLoop();
    void hashTableInsert(std::pair<std::string, SharedConnectionPtr>& pair_val);
    void hashTableRemove(std::string& uuid);

private:
    std::string service_name_;
    ServerAddress server_address_;
    std::unique_ptr<EventLoop> control_event_loop_;
    std::unique_ptr<Listener> listener_;
    std::unique_ptr<EventLoopPool> event_loop_thread_pool_;
    std::unique_ptr<DynamicThreadPool> dynamic_thread_pool_;
    std::unique_ptr<UUIDGenerator> uuid_generator_;
    std::unique_ptr<Codec> json_codec_;
    std::unique_ptr<HashTableConn> conns_table_;

    ConnectionCallback conn_fn_;
    ReadMessageCallback message_fn_;
    int thread_num_;
};

} /* end namespace atp */;

#endif /* __SERVICE_H__ */
