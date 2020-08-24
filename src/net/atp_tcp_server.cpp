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

#include <sys/sysinfo.h>

#include "net/atp_config.h"
#include "net/atp_tcp_conn.h"
#include "net/atp_listener.h"
#include "net/atp_tcp_server.h"
#include "net/atp_dynamic_thread_pool.h"
#include "app/atp_uuid.h"

namespace atp {

/*
 * Get current available CPU for event loop count.
 */
int getSystemCPUProcessers() {
    return get_nprocs();
}

Server::Server(std::string name, ServerAddress server_address, int thread_num) {
    /*
     * Initialize all components for tcp server.
     */
    state_.store(STATE_NULL);
    service_name_ = name;
    server_address_ = server_address;
    thread_num_ = thread_num;
    dynamic_thread_pool_size_ = getSystemCPUProcessers() * 2;
    server_mode_ = 0;

    control_event_loop_.reset(new EventLoop());
    listener_.reset(new Listener(control_event_loop_.get(), server_address_.addr_, server_address_.port_));
    dynamic_thread_pool_.reset(new DynamicThreadPool(thread_num_));
    uuid_generator_.reset(new UUIDGenerator());
    json_codec_.reset(new Codec());
    conns_table_.reset(new HashTableConn());

    if (thread_num_ > 0) {
        event_loop_thread_pool_.reset(new EventLoopPool(thread_num_));
    }

    assert(control_event_loop_ != nullptr);
    assert(listener_ != nullptr);
    assert(dynamic_thread_pool_ != nullptr);
    assert(uuid_generator_ != nullptr);
    assert(json_codec_ != nullptr);
    assert(conns_table_ != nullptr);
    assert(server_address_.addr_.length() != 0);
    assert(server_address_.port_ > 0);

    if (thread_num_ > 0) {
        assert(event_loop_thread_pool_ != nullptr);
    }

    if (service_name_.length() == 0) {
        service_name_ = "SERVER-" + uuid_generator_->generateUUID();
    }

    assert(service_name_.length() != 0);

    if (ATP_NET_DEBUG_ON) {
        LOG(INFO) << "[Server] create server: " << service_name_;
    }

    state_.store(STATE_INIT);
}

Server::Server(std::string name, EventLoop* event_loop, ServerAddress server_address, int thread_num) {
    /* Initialize all components for the server. */
    state_.store(STATE_NULL);
    service_name_ = name;
    server_address_ = server_address;
    thread_num_ = thread_num;
    dynamic_thread_pool_size_ = getSystemCPUProcessers() * 2;
    server_mode_ = 1;

    control_event_loop_.reset(event_loop);
    listener_.reset(new Listener(control_event_loop_.get(), server_address_.addr_, server_address_.port_));
    dynamic_thread_pool_.reset(new DynamicThreadPool(thread_num_));
    uuid_generator_.reset(new UUIDGenerator());
    json_codec_.reset(new Codec());
    conns_table_.reset(new HashTableConn());

    if (thread_num_ > 0) {
        event_loop_thread_pool_.reset(new EventLoopPool(thread_num_));
    }

    assert(control_event_loop_ != nullptr);
    assert(listener_ != nullptr);
    assert(dynamic_thread_pool_ != nullptr);
    assert(uuid_generator_ != nullptr);
    assert(json_codec_ != nullptr);
    assert(conns_table_ != nullptr);
    assert(server_address_.addr_.length() != 0);
    assert(server_address_.port_ > 0);

    if (thread_num_ > 0) {
        assert(event_loop_thread_pool_ != nullptr);
    }

    if (service_name_.length() == 0) {
        service_name_ = "SERVER-" + uuid_generator_->generateUUID();
    }

    assert(service_name_.length() != 0);

    if (ATP_NET_DEBUG_ON) {
        LOG(INFO) << "[Server] create server: " << service_name_;
    }

    state_.store(STATE_INIT);
}

Server::~Server() {
    state_.store(STATE_STOPPED);
}

void Server::start() {
    /* Start listener to listenning baddr and port. */
    listener_->listen();

    /* Start event_loop_pool, it mabe had none event_loop_thread. */
    startEventLoopPool();

    /* Bind listener server layer handle. */
    listener_->setNewConnCallback(std::bind(&Server::handleNewConnection, this,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    listener_->accept();

    /* Update server state to running. */
    state_.store(STATE_RUNNING);

    if (!server_mode_) {
        control_event_loop_->dispatch();
    }
}

void Server::stop() {
    state_.store(STATE_STOPPING);
    stopEventLoopPool();
    state_.store(STATE_STOPPED);
}

void Server::startEventLoopPool() {
    if (thread_num_ > 0) {
        event_loop_thread_pool_->autoStart();
    }
}

void Server::stopEventLoopPool() {
    if (thread_num_ > 0) {
        event_loop_thread_pool_->autoStop();
    }
}

void Server::handleNewConnection(int fd, std::string& taddr, void* args) {
    assert(CHECK_STATE(STATE_RUNNING));

    EventLoop* event_loop = nullptr;
    thread_num_ == 0 ? event_loop = control_event_loop_.get() : event_loop = getIOEventLoop();
    assert(event_loop != nullptr);

    ConnectionPtr conn(new Connection(event_loop, fd, uuid_generator_->generateUUID(), taddr));
    conn->setConnectionCallback(conn_fn_);
    conn->setReadMessageCallback(message_fn_);
    conn->setCloseCallback(std::bind(&Server::handleCloseConnection, this, std::placeholders::_1));

    assert(conn);

    event_loop->sendToQueue(std::bind(&Connection::attachToEventLoop, conn));
    std::pair<std::string, ConnectionPtr> pair_value;
    pair_value = std::make_pair(conn->getUUID(), conn);
    hashTableInsert(pair_value);
}

void Server::handleCloseConnection(const ConnectionPtr& conn) {
    auto fn = [this, conn]() {
        this->hashTableRemove(conn->getUUID());
    };

    /*
     * Thinking about this why used control_event_loop_ not used conn event_loop it self.
     * We only have one container to store connections And want to reduce the lib lock.
     */
    control_event_loop_->sendToQueue(fn);
}

EventLoop* Server::getIOEventLoop() {
    assert(CHECK_STATE(STATE_RUNNING));
    return event_loop_thread_pool_->getIOEventLoop();
}

size_t Server::hashTableSize() {
    return conns_table_->size();
}

void Server::hashTableInsert(std::pair<std::string, ConnectionPtr>& pair_val) {
    if (ATP_NET_DEBUG_ON) {
        LOG(INFO) << "[hashTableInsert] conn uuid: " << pair_val.first;
    }

    conns_table_->insert(pair_val);
}

void Server::hashTableRemove(std::string uuid) {
    if (ATP_NET_DEBUG_ON) {
        LOG(INFO) << "[hashTableRemove] conn uuid: " << uuid;
    }

    conns_table_->erase(uuid);
}

} /* end namespace atp */
