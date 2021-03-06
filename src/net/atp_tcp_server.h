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

#ifndef __SERVICE_H__
#define __SERVICE_H__

#include "net/atp_cbs.h"
#include "net/atp_tcp_conn.h"
#include "net/atp_event_loop.h"
#include "net/atp_event_loop_thread_pool.h"
#include "net/atp_timing_wheel.hpp"
#include "net/atp_state_machine.hpp"
#include "app/atp_codec.hpp"

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

    void timingWheelInsert(EntryPtr& entry) {
        LOG(INFO) << "timing insert use1 count: " << entry.use_count();
        if (!server_mode_) {
            std::lock_guard<std::mutex> lock(lock_);
            timing_wheel_->push_back(entry);
        } else {
            timing_wheel_->push_back(entry);
        }

        LOG(INFO) << "timing insert use2 count: " << entry.use_count();
    }

private:
    void doInit();

    void startEventLoopPool();

    void stopEventLoopPool();

    void handleNewConnection(int fd, std::string& taddr, void* args);

    void handleCloseConnection(const ConnectionPtr& conn);

    EventLoop* getIOEventLoop();

    size_t hashTableSize();

    void hashTableInsert(std::pair<std::string, ConnectionPtr>& pair_val);

    void hashTableRemove(std::string uuid);

private:
    // The TCP server name.
    std::string service_name_;

    // Only support IPV4 address.
    ServerAddress server_address_;

    // The control event loop send connection to IO event loop.
    std::unique_ptr<EventLoop> control_event_loop_;

    // TCP listener for accept new connection.
    std::unique_ptr<Listener> listener_;

    // Each threads with one IO event loop.
    std::unique_ptr<EventLoopPool> event_loop_thread_pool_;

    // Scalable thread pool.
    std::unique_ptr<DynamicThreadPool> dynamic_thread_pool_;

    // Generate uuid for each connections.
    std::unique_ptr<UUIDGenerator> uuid_generator_;

    // The code for json encode/decode.
    std::unique_ptr<Codec> json_codec_;

    // The container storage all established connections.
    std::unique_ptr<HashTableConn> conns_table_;

    // The timing wheel manage all established connections tiemout.
    std::unique_ptr<TimingWheel> timing_wheel_;

    // The timing wheel lock.
    std::mutex lock_;

    // Incoming a connection will be call this.
    ConnectionCallback conn_fn_;

    // Connection had message will call this.
    ReadMessageCallback message_fn_;

    // Connection clsoed will call this.
    CloseCallback close_fn_;

    // The event loop pool core size.
    int thread_num_;

    // Dynamic thread pool core thread size.
    int dynamic_thread_pool_size_;

    // Server mode for SO_REUSEPORT or not.
    int server_mode_;
};

} /* end namespace atp */;

#endif /* __SERVICE_H__ */
