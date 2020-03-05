#include <fcntl.h>

#include "atp_debug.h"
#include "atp_socket.h"
#include "atp_libevent.h"
                                                        
namespace atp {

static int get_socket_nonblocking(int fd);

static int get_socket_nonblocking(int fd) {
    int mask = fcntl(fd, F_GETFL, NULL);

    int opt_val = mask & O_NONBLOCK;

    return (opt_val == 0);
}

bool SocketImpl::getOption(int opt_id) {
    int opt_val = 0;
    socklen_t opt_len = sizeof(opt_val);
    
    switch (opt_id) {
    case O_NONBLOCK:
        opt_val = get_socket_nonblocking(fd_);
        break;
    case SO_REUSEADDR:
        getsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt_val, &opt_len);
        break;
    case SO_REUSEPORT:
        getsockopt(fd_, SOL_SOCKET, SO_REUSEPORT, &opt_val, &opt_len);
        break;
    case TCP_DEFER_ACCEPT:
        getsockopt(fd_, IPPROTO_TCP, TCP_DEFER_ACCEPT, &opt_val, &opt_len);
        break;
    case TCP_NODELAY:
        getsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, &opt_val, &opt_len);
        break;
    case TCP_QUICKACK:
        getsockopt(fd_, IPPROTO_TCP, TCP_QUICKACK, &opt_val, &opt_len);
        break;
    default:
        return false;
    }
    
    return (opt_val == 1);
}

void SocketImpl::setOption(int opt_id, int on) {
    switch (opt_id) {
    case O_NONBLOCK:
        on == 1 ? evutil_make_socket_nonblocking(fd_) : 0;
        break;

    case SO_REUSEADDR:
        on == 1 ? evutil_make_listen_socket_reuseable(fd_) : 0;
        break;

    case SO_REUSEPORT:
        on == 1 ? evutil_make_listen_socket_reuseable_port(fd_) : 0;
        break;

    case TCP_DEFER_ACCEPT:
        on == 1 ? evutil_make_tcp_listen_socket_deferred(fd_) : 0;
        break;
    
    case TCP_NODELAY:
        setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, &on, static_cast<socklen_t>(sizeof(on)));
        break;

    case TCP_QUICKACK:
        setsockopt(fd_, IPPROTO_TCP, TCP_QUICKACK, &on, static_cast<socklen_t>(sizeof(on)));
        break;
/*
    // TCP_DEFER_ACCEPT == SO_KEEPALIVE
    case SO_KEEPALIVE:
        setsockopt(fd_, SOL_SOCKET, SO_KEEPALIVE, &on, static_cast<socklen_t>(sizeof(on)));
        break;
*/
    }
}

void SocketImpl::setfd(int fd) {
    fd_ = fd;
}

int SocketImpl::create(bool stream) {
    if (stream) {
        fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
        if (fd_ < 0) {
            return -1;
        }

        if (evutil_make_socket_nonblocking(fd_) < 0) {
            goto error;
        }
        
        if (fcntl(fd_, F_SETFD, 1) == - 1) {
            goto error;
        }

        return fd_;
    }

    /* Current can't suport other socket type */
error:
    ::close(fd_);
    fd_ = 0;
    return -1;
}

int SocketImpl::connect(std::string& ip, int port) {
    return 0;
}

int SocketImpl::bind(std::string& ip, int port) {
    ip_ = ip;
    port_ = port;
    
    struct sockaddr_in baddr;
    memset(&baddr, 0, sizeof(baddr));

    /* Set listen address and port */
    baddr.sin_family = AF_INET;
    baddr.sin_port = htons(port_);
    baddr.sin_addr.s_addr = inet_addr(ip_.c_str());
        
    if (::bind(fd_, (struct sockaddr*)&baddr, sizeof(baddr)) < 0) {
        return -1;
    }

    return 0;
}

int SocketImpl::listen(int backlog) {
    int SO_MAX_CONN = 0;
    backlog <= 0 ? SO_MAX_CONN = ATP_SO_MAX_CONN : SO_MAX_CONN = backlog;
    if (::listen(fd_, SO_MAX_CONN) < 0) {
        return -1;
    }

    return 0;
}

int SocketImpl::accept(std::string& remote_addr) {
    struct sockaddr_in raddr;
    socklen_t addr_len = sizeof(raddr);
    int conn_fd = ::accept(fd_, reinterpret_cast<struct sockaddr*>(&raddr), &addr_len);
    return conn_fd;
}

void SocketImpl::close() {
    ::close(fd_);
    fd_ = -1;
}

}/*end namespace atp*/
