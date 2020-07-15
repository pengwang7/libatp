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

bool SocketImpl::getOption(int fd, int opt_id) {
    int opt_val = 0;
    socklen_t opt_len = sizeof(opt_val);
    
    switch (opt_id) {
    case O_NONBLOCK:
        opt_val = get_socket_nonblocking(fd);
        break;
    case SO_REUSEADDR:
        getsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, &opt_len);
        break;
    case SO_REUSEPORT:
        getsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &opt_val, &opt_len);
        break;
    case TCP_DEFER_ACCEPT:
        getsockopt(fd, IPPROTO_TCP, TCP_DEFER_ACCEPT, &opt_val, &opt_len);
        break;
    case TCP_NODELAY:
        getsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &opt_val, &opt_len);
        break;
    case TCP_QUICKACK:
        getsockopt(fd, IPPROTO_TCP, TCP_QUICKACK, &opt_val, &opt_len);
        break;
    default:
        return false;
    }
    
    return (opt_val == 1);
}

void SocketImpl::setOption(int fd, int opt_id, int on) {
    switch (opt_id) {
    case O_NONBLOCK:
        on == 1 ? evutil_make_socket_nonblocking(fd) : 0;
        break;

    case SO_REUSEADDR:
        on == 1 ? evutil_make_listen_socket_reuseable(fd) : 0;
        break;

    case SO_REUSEPORT:
        on == 1 ? evutil_make_listen_socket_reuseable_port(fd) : 0;
        break;

    case TCP_DEFER_ACCEPT:
        on == 1 ? evutil_make_tcp_listen_socket_deferred(fd) : 0;
        break;
    
    case TCP_NODELAY:
        setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &on, static_cast<socklen_t>(sizeof(on)));
        break;

    case TCP_QUICKACK:
        setsockopt(fd, IPPROTO_TCP, TCP_QUICKACK, &on, static_cast<socklen_t>(sizeof(on)));
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
    return SOCKET_CREATE_ERROR;
}

int SocketImpl::connect(std::string& ip, int port, struct timeval* tv) {
    int ret = 0;
    int res = 0;

    struct sockaddr_in srvaddr;
    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_port = htons(port);
    srvaddr.sin_addr.s_addr = inet_addr(ip.c_str());

    do {
        ret = ::connect(fd_, reinterpret_cast<struct sockaddr*>(&srvaddr), sizeof(srvaddr));
    } while (ret < 0 && errno == EINTR);

    if (ret == 0) {
        return 0;
    }

    if (ret < 0 && errno == EINPROGRESS) {
        fd_set conn_fdset, error_fdset;
        FD_ZERO(&conn_fdset);
        FD_ZERO(&error_fdset);
        FD_SET(fd_, &conn_fdset);
        FD_SET(fd_, &error_fdset);

        res = select(fd_ + 1, NULL, &conn_fdset, &error_fdset, tv);
        if (res < 0) {
            close();

            return SOCKET_CONNECT_ERROR;
        } else if (res == 0) {
            errno = ETIMEDOUT;
            close();

            return SOCKET_CONNECT_TIMEOUT;
        }

        int so_error;
        socklen_t so_error_len = sizeof(so_error);

        if (FD_ISSET(fd_, &error_fdset)) {
            getsockopt(fd_, SOL_SOCKET, SO_ERROR, &so_error, &so_error_len);
            errno = so_error;
            close();

            return SOCKET_GET_APP_ERROR;
        }
    } else {
        return SOCKET_CONNECT_ERROR;
    }

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
        return SOCKET_BIND_ERROR;
    }

    return 0;
}

int SocketImpl::listen(int backlog) {
    int SO_MAX_CONN = 0;
    backlog <= 0 ? SO_MAX_CONN = ATP_SO_MAX_CONN : SO_MAX_CONN = backlog;
    if (::listen(fd_, SO_MAX_CONN) < 0) {
        return SOCKET_LISTEN_ERROR;
    }

    return 0;
}

int SocketImpl::accept(std::string& remote_addr) {
    char buf[INET_ADDRSTRLEN];
    struct sockaddr_in raddr;
    socklen_t addr_len = sizeof(raddr);
    int conn_fd = ::accept(fd_, reinterpret_cast<struct sockaddr*>(&raddr), &addr_len);
    if (conn_fd < 0) {
        return SOCKET_ACCEPT_ERROR;
    }

    remote_addr = inet_ntop(raddr.sin_family, &raddr.sin_addr, buf, sizeof(buf));

    return conn_fd;
}

void SocketImpl::close() {
    ::close(fd_);
    fd_ = -1;
}

}/*end namespace atp*/
