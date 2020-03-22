#include <sys/sysinfo.h>

#include "atp_uuid.h"
#include "atp_tcp_conn.h"
#include "atp_listener.h"
#include "atp_event_loop.h"
#include "atp_dynamic_thread_pool.h"
#include "atp_event_loop_thread_pool.h"
#include "atp_tcp_server.h"

namespace atp {

Server::Server(std::string name, ServerAddress server_address, int thread_num) {
    /* Initialize all components for the server. */
    state_.store(STATE_NULL);
    service_name_ = name;
    server_address_ = server_address;
    thread_num_ = thread_num;

    if (thread_num_ < 0) {
        thread_num_ = getSystemCPUProcessers() * 2;
    }

    control_event_loop_.reset(new EventLoop());
    listener_.reset(new Listener(control_event_loop_.get(), server_address_.addr_, server_address_.port_));
    event_loop_thread_pool_.reset(new EventLoopPool(thread_num_));
    dynamic_thread_pool_.reset(new DynamicThreadPool(thread_num_));
    uuid_generator_.reset(new UUIDGenerator());
    json_codec_.reset(new Codec());
    conns_table_.reset(new HashTableConn());

    assert(control_event_loop_ != nullptr);
    assert(listener_ != nullptr);
    assert(event_loop_thread_pool_ != nullptr);
    assert(dynamic_thread_pool_ != nullptr);
    assert(uuid_generator_ != nullptr);
    assert(json_codec_ != nullptr);
    assert(conns_table_ != nullptr);
    assert(server_address_.addr_.length() != 0);
    assert(server_address_.port_ > 0);

    if (service_name_.length() == 0) {
        service_name_ = "SERVER-" + uuid_generator_->generateUUID();
    }

    assert(service_name_.length() != 0);

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

    control_event_loop_->dispatch();
}

void Server::stop() {
    state_.store(STATE_STOPPING);
    stopEventLoopPool();
    state_.store(STATE_STOPPED);
}

int Server::getSystemCPUProcessers() {
    /* Get current available CPU. */
    return get_nprocs();
}

void Server::startEventLoopPool() {
    if (event_loop_thread_pool_->autoStart()) {
        /* One listener mode. */
    } else {
        /* Multi listener mode. */
    }
}

void Server::stopEventLoopPool() {
    event_loop_thread_pool_->autoStop();
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

	/* Thinking about why used control_event_loop_ not used conn event_loop it self. */
	/* We only have one container to store connections And want to reduce the lib lock. */
    control_event_loop_->sendToQueue(fn);
}

EventLoop* Server::getIOEventLoop() {
    assert(CHECK_STATE(STATE_RUNNING));
    return event_loop_thread_pool_->getIOEventLoop();
}

void Server::hashTableInsert(std::pair<std::string, ConnectionPtr>& pair_val) {
    conns_table_->insert(pair_val);
}

void Server::hashTableRemove(std::string uuid) {
    conns_table_->erase(uuid);
}

} /* end namespace atp */
