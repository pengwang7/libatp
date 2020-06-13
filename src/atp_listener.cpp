#include "atp_debug.h"
#include "atp_socket.h"
#include "atp_listener.h"
#include "atp_channel.h"
#include "atp_event_loop.h"
#include "glog/logging.h"

namespace atp {

Listener::Listener(EventLoop* event_loop, const std::string& address, unsigned int port)
    : event_loop_(event_loop), listen_fd_(-1) {
    address_.host_ = address;
    address_.port_ = port;
}

Listener::~Listener() {
    ::close(listen_fd_);
    listen_fd_ = -1;
}

void Listener::listen() {
    listen_fd_ = create(true);
    assert(listen_fd_ >= 0);

    /* Set socket flags for listen fd */
    setOption(listen_fd_, SO_REUSEADDR, 1);
    setOption(listen_fd_, SO_REUSEPORT, 1);
    setOption(listen_fd_, TCP_DEFER_ACCEPT, 1);

    assert(bind(address_.host_, address_.port_) == 0);
    assert(SocketImpl::listen(ATP_SO_MAX_CONN) == 0);

    if (ATP_DEBUG_ON) {
        LOG(INFO) << "Listener listen host: " << address_.host_ << "  port: " << address_.port_;
    }
}

void Listener::accept() {
    channel_.reset(new Channel(event_loop_, listen_fd_, true, false));
    channel_->setReadCallback(std::bind(&Listener::acceptHandle, this));

    /* Attach listen channel event to owner event loop */
    event_loop_->sendToQueue(std::bind(&Channel::attachToEventLoop, channel_.get()));
}

void Listener::stop() {
    assert(event_loop_->safety());
    channel_->disableAllEvents();
    channel_->close();
}

void Listener::acceptHandle() {
    assert(event_loop_->safety());

    std::string remote_address;
    int conn_fd = SocketImpl::accept(remote_address);
    if (conn_fd < 0) {
        if (errno != EAGAIN && errno != EINTR) {
            LOG(ERROR) << "Accept connection met error: " << strerror(errno);
        }

        return;
    }

    if (ATP_DEBUG_ON) {
        LOG(INFO) << "The listener accept fd thread id: " << std::this_thread::get_id();
    }

	/* TCP_NODELAY and TCP_QUICKACK are need to used together */
    setOption(conn_fd, O_NONBLOCK, 1);
    setOption(conn_fd, TCP_NODELAY, 1);
    setOption(conn_fd, TCP_QUICKACK, 1);
    
    /* Notify application layer accept a new connectoin */
    if (new_conn_cb_) {
        new_conn_cb_(conn_fd, remote_address, NULL);
    }
}

}/*end namespace atp*/
