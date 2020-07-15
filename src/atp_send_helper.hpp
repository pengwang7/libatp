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

#ifndef __ATP_SEND_HELPER_HPP__
#define __ATP_SEND_HELPER_HPP__

#include <poll.h>

#include "atp_socket.h"
#include "glog/logging.h"


namespace atp {

typedef enum {
    ATP_HELPER_ERROR_BASE            =  (-100),
    ATP_HELPER_CREATE_SOCKET_ERROR   =  (ATP_HELPER_ERROR_BASE - 1),
    ATP_HELPER_CONNECT_SOCKET_ERROR  =  (ATP_HELPER_ERROR_BASE - 2),
    ATP_HELPER_INVALID_ARGS          =  (ATP_HELPER_ERROR_BASE - 3),
    ATP_HELPER_NONBLOCK_WRITE_ERROR  =  (ATP_HELPER_ERROR_BASE - 4),
    ATP_HELPER_NONBLOCK_READ_ERROR   =  (ATP_HELPER_ERROR_BASE - 5),
    ATP_HELPER_POLL_ERROR            =  (ATP_HELPER_ERROR_BASE - 6),
    ATP_HELPER_SELECT_ERROR          =  (ATP_HELPER_ERROR_BASE - 7),
    ATP_HELPER_SEND_ERROR            =  (ATP_HELPER_ERROR_BASE - 8),
    ATP_HELPER_RECV_ERROR            =  (ATP_HELPER_ERROR_BASE - 9)
} PbxHelperErrorCode;
    
class Socket : public SocketImpl {
public:
    Socket() {}

    virtual ~Socket() {}

public:
    int create(bool tcp_stream) {
        return SocketImpl::create(tcp_stream);
    }

    int connect(std::string ip, int port, struct timeval* tv) {
        return SocketImpl::connect(ip, port, tv);
    }
};

/* The SendHelper with nonblocking socket and */
/* In the form of short links help us send message to remote. */
class SendHelper {
public:
    SendHelper() {
        fd_ = -1;
        retry_count_ = 0;
    }

    ~SendHelper() {
        if (fd_ >= 0) {
            close(fd_);
            fd_ = -1;
        }

        retry_count_ = 0;
    }

public:
    /* Support external fd. */
    int setFd(int fd) {
        fd_ = fd;

        return fd_;
    }

    void setRetryCount(int retry_count) {
        retry_count_ = retry_count;
    }

    /* The interface used the nonblocking socket try to connect/read/write. */
    int sendMessage(std::string ip, int port, std::string message, struct timeval* tv = NULL) {
        if (message.size() <= 0) {
            return ATP_HELPER_INVALID_ARGS;
        }

        Socket sock;
        fd_ = sock.create(false);
        if (fd_ != 0) {
            return ATP_HELPER_CREATE_SOCKET_ERROR;
        }

        // The address for test
        int ret = sock.connect(ip, port, tv);
        if (ret != 0) {
            LOG(ERROR) << "Socket connect error: " << strerror(errno);

            return ATP_HELPER_CONNECT_SOCKET_ERROR;
        }

        ret = nonblockWrite(message, tv);
        if (ret != 0) {
            return ATP_HELPER_NONBLOCK_WRITE_ERROR;
        }

        std::string result;
        ret = nonblockRead(result, tv);
        if (ret <= 0) {
            return ATP_HELPER_NONBLOCK_READ_ERROR;
        }

        LOG(INFO) << "Read remote data: " << result;

        return 0;
    }

private:
    int nonblockRead(std::string& data, struct timeval* tv) {
        int ret = 0;

        struct pollfd pollfds;

        do {
            pollfds.fd = fd_;
            pollfds.events = POLLIN;

            ret = poll(&pollfds, 1, tv == NULL ? NULL : tv->tv_sec);
        } while ((ret < 0 && (errno == EINTR || errno == EINPROGRESS)));

        if (ret <= 0) {
            if (ret == 0) {
                errno = ETIMEDOUT;
            }

            LOG(INFO) << "The poll met error: " << strerror(errno);
            close(fd_);
            fd_ = -1;

            return ATP_HELPER_POLL_ERROR;
        }

        if (pollfds.revents & ((POLLERR | POLLNVAL))) {
            close(fd_);
            fd_ = -1;

            return ATP_HELPER_POLL_ERROR;
        }

        if (pollfds.revents & (POLLIN | POLLPRI | POLLRDHUP)) {
            char buff[4096];
            memset(buff, 0, sizeof(buff));
            ret = recv(fd_, buff, sizeof(buff), 0);
            if (ret <= 0) {
                LOG(INFO) << "The recv met error: " << strerror(errno);

                close(fd_);
                fd_ = -1;

                return ATP_HELPER_RECV_ERROR;
            }

            data = std::string(buff);
        }

        return ret;
    }

    int nonblockWrite(std::string data, struct timeval* tv) {
        ssize_t nwrite = 0;
        size_t remaining_data_size = data.size();

        nwrite = ::send(fd_, static_cast<const char*>(data.c_str()), data.size(), MSG_NOSIGNAL);
        if (nwrite >= 0) {
            remaining_data_size -= nwrite;
            if (remaining_data_size == 0) {
                return 0;
            }
        } else {
            close(fd_);
            fd_ = -1;

            return ATP_HELPER_SEND_ERROR;
        }

        if (remaining_data_size > 0) {
            int ret = 0;
            int offset = nwrite;

            do {
                struct pollfd pollfds;
                pollfds.fd = fd_;
                pollfds.events = POLLOUT;

                ret = poll(&pollfds, 1, tv == NULL ? NULL : tv->tv_sec);
                if (ret < 0 && errno == EINTR) {
                    continue;
                } else {
                    return ATP_HELPER_POLL_ERROR;
                }

                if (pollfds.revents & (POLLERR | POLLNVAL | POLLHUP)) {
                    close(fd_);
                    fd_ = -1;

                    return ATP_HELPER_POLL_ERROR;
                }

                if (pollfds.revents & POLLOUT) {
                    nwrite = ::send(fd_, static_cast<const char*>(data.c_str() + offset), remaining_data_size, MSG_NOSIGNAL);
                    if (nwrite < 0) {
                        return ATP_HELPER_SEND_ERROR;
                    }

                    if (nwrite >= 0) {
                        remaining_data_size -= nwrite;

                        if (remaining_data_size == 0) {
                            return 0;
                        } else {
                            offset += nwrite;
                        }
                    }
               }
           } while (true);
        }

        return ATP_HELPER_SEND_ERROR;
    }

private:
    int fd_;
    int retry_count_;
};

} /* end namespace atp */

#endif /* __ATP_SEND_HELPER_HPP__ */
