
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

#include "net/atp_config.h"
#include "net/atp_channel.h"
#include "net/atp_listener.h"
#include "net/atp_event_loop.h"

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

    // Set socket flags for listen fd.
    setOption(listen_fd_, SO_REUSEADDR, 1);
    setOption(listen_fd_, SO_REUSEPORT, 1);
    setOption(listen_fd_, TCP_DEFER_ACCEPT, 1);

    assert(bind(address_.host_, address_.port_) == 0);
    assert(SocketImpl::listen(ATP_SO_MAX_CONN) == 0);

    if (ATP_NET_DEBUG_ON) {
        LOG(INFO) << "[Listener] listen host: " << address_.host_ << "  port: " << address_.port_;
    }
}

void Listener::accept() {
    channel_.reset(new Channel(event_loop_, listen_fd_, true, false));
    channel_->setReadCallback(std::bind(&Listener::acceptHandle, this));

    /* Attach listen channel event to owner event loop. */
    event_loop_->sendToQueue(std::bind(&Channel::attachToEventLoop, channel_.get()));
}

void Listener::stop() {
    assert(event_loop_->threadSafety());
    channel_->disableAllEvents();
    channel_->close();
}

void Listener::acceptHandle() {
    assert(event_loop_->threadSafety());

    std::string remote_address;
    int conn_fd = SocketImpl::accept(remote_address);
    if (conn_fd < 0) {
        if (errno != EAGAIN && errno != EINTR) {
            LOG(ERROR) << "[Listener] acceptHandle accept connection met error: " << strerror(errno);
        }

        return;
    }

    if (ATP_NET_DEBUG_ON) {
        LOG(INFO) << "[Listener] acceptHandle listener accept fd thread id: " << std::this_thread::get_id();
    }

	// TCP_NODELAY and TCP_QUICKACK are need to used together.
    setOption(conn_fd, O_NONBLOCK, 1);
    setOption(conn_fd, TCP_NODELAY, 1);
    setOption(conn_fd, TCP_QUICKACK, 1);

    // Notify application layer accept a new connectoin.
    if (new_conn_cb_) {
        new_conn_cb_(conn_fd, remote_address, NULL);
    }
}

} /* end namespace atp */
