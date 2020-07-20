#ifndef __SERVICE_H__
#define __SERVICE_H__

/*
 * MIT License
 *
 * Copyright (c) 2019 pengwang7(https://github.com/pengwang7/libatp)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <sstream>

#include "jsoncpp/json.h"

#include "atp_cbs.h"
#include "atp_tcp_conn.h"
#include "atp_event_loop.h"
#include "atp_event_loop_thread_pool.h"
#include "atp_state_machine.h"

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

int getSystemCPUProcessers();

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

class Server : public STATE_MACHINE_INTERFACE {
public:
    explicit Server(std::string name, ServerAddress server_address, int thread_nums);

    /* Multi accept mode(SO_REUSEPORT) */
    explicit Server(std::string name, EventLoop* event_loop, ServerAddress server_address, int thread_num);

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
    void startEventLoopPool();
    void stopEventLoopPool();
    void handleNewConnection(int fd, std::string& taddr, void* args);
    void handleCloseConnection(const ConnectionPtr& conn);
    EventLoop* getIOEventLoop();
    size_t hashTableSize();
    void hashTableInsert(std::pair<std::string, ConnectionPtr>& pair_val);
    void hashTableRemove(std::string uuid);

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
    CloseCallback close_fn_;
    int thread_num_;
    int dynamic_thread_pool_size_;
    int server_mode_;
};

} /* end namespace atp */;

#endif /* __SERVICE_H__ */
