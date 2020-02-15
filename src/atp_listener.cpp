#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "glog/logging.h"

#include "atp_debug.h"
#include "atp_socket.h"
#include "atp_listener.h"
#include "atp_channel.h"
#include "atp_event_loop.h"

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

void Listener::listening() {
#if 0
    fd_ = socket::createNonblockingSocket();
    assert(fd_ != -1);

    /* Set listen socket fd options */
    socket::setReuseAddr(fd_);
    socket::setReusePort(fd_);
    socket::setTCPDeferred(fd_);
    socket::setKeepalive(fd_, true);

    struct sockaddr_in baddr;
    memset(&baddr, 0, sizeof(baddr));

    /* Set listen address and port */
    baddr.sin_family = AF_INET;
    baddr.sin_port = htons(address_.port_);
    baddr.sin_addr.s_addr = inet_addr(address_.host_.c_str());
        
    if (bind(fd_, (struct sockaddr*)&baddr, sizeof(baddr)) < 0) {
        LOG(ERROR) << "Listen socket bind met error: " << strerror(errno);
        return;
    }

    if (listen(fd_, ATP_SO_MAX_CONN) < 0) {
        LOG(ERROR) << "Listen met error: " << strerror(errno);
        return;
    }

    if (ATP_DEBUG_ON) {
        LOG(INFO) << "Listen success, fd: " << fd_;
    }
#endif
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
#if 0
    struct sockaddr_in raddr;
    socklen_t addr_len = sizeof(raddr);
    int conn_fd = ::accept(fd_, reinterpret_cast<struct sockaddr*>(&raddr), &addr_len);
    if (conn_fd == -1) {
        if (errno != EAGAIN && errno != EINTR) {
            LOG(ERROR) << "The accept met error: " << strerror(errno);
        }
        return;
    }

    socket::setNonblocking(conn_fd);
    socket::setTCPNoDelay(conn_fd, true);
    socket::setQuickAck(conn_fd, true);

    /* Notify application layer accept a new connection */
    if (new_conn_cb_) {
        new_conn_cb_(conn_fd, std::string(""), NULL);
    }
#endif
}

}/*end namespace atp*/
