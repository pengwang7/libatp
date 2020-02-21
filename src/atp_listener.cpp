#include "atp_debug.h"
#include "atp_socket.h"
#include "atp_listener.h"
#include "atp_channel.h"
#include "atp_event_loop.h"
#include "glog/logging.h"

namespace atp {

Listener::Listener(EventLoop* event_loop, const std::string& address, unsigned int port)
    : event_loop_(event_loop), fd_(-1) {
    address_.host_ = address;
    address_.port_ = port;
}

Listener::~Listener() {
    ::close(fd_);
    fd_ = -1;
}

void Listener::listen() {
    fd_ = this->create(true);
    assert(fd_ != -1);

    /* Setl socket flags for listen fd */
    this->setfd(fd_);
    this->setOption(SO_REUSEADDR, 1);
    this->setOption(SO_REUSEPORT, 1);
    this->setOption(TCP_DEFER_ACCEPT, 1);

    assert(this->bind(address_.host_, address_.port_) == 0);
    assert(this->SocketImpl::listen(ATP_SO_MAX_CONN) == 0);
    
    if (ATP_DEBUG_ON) {
        LOG(INFO) << "The Server listen fd: " << fd_ << " address: " << address_.host_ << ":" << address_.port_;
    }
}

void Listener::accept() {
    channel_.reset(new Channel(event_loop_, fd_, true, false));
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
    int conn_fd = this->SocketImpl::accept(remote_address);
    if (conn_fd == -1) {
        if (errno != EAGAIN && errno != EINTR) {
            LOG(ERROR) << "Accept connection met error: " << strerror(errno);
        }

        return;
    }

    this->setfd(conn_fd);
    this->setOption(O_NONBLOCK, 1);
    this->setOption(TCP_NODELAY, 1);
    this->setOption(TCP_QUICKACK, 1);
    
    /* Notify application layer accept a new connectoin */
    if (new_conn_cb_) {
        new_conn_cb_(conn_fd, remote_address, NULL);
    }
}

}/*end namespace atp*/
