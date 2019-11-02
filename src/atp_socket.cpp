#include "atp_socket.h"
#include "atp_libevent.h"

namespace atp {

namespace socket {

int createNonblockingSocket() {
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        return -1;
    }

    if (evutil_make_socket_nonblocking(fd) < 0) {
        goto error;
    }

    if (fcntl(fd, F_SETFD, 1) == - 1) {
        goto error;
    }

    return fd;

error:
    close(fd);
    fd = 0;
    return -1;
}

void setNonblocking(int fd) {
    evutil_make_socket_nonblocking(fd);
}

void setReuseAddr(int fd) {
    evutil_make_listen_socket_reuseable(fd);
}

void setReusePort(int fd) {
    evutil_make_listen_socket_reuseable_port(fd);
}

void setTCPDeferred(int fd) {
    evutil_make_tcp_listen_socket_deferred(fd);
}

void setTCPNoDelay(int fd, bool on) {
    int optval = on ? 1 : 0;
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &optval, static_cast<socklen_t>(sizeof(optval)));
}

void setQuickAck(int fd, bool on) {
    int optval = on ? 1 : 0;                                                     
    setsockopt(fd, IPPROTO_TCP, TCP_QUICKACK, &optval, static_cast<socklen_t>(sizeof(optval)));
}

void setKeepalive(int fd, bool on) {
    int optval = on ? 1 : 0;
    setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &optval, static_cast<socklen_t>(sizeof(optval)));
}

}/*end namespace socket*/

}/*end namespace atp*/
