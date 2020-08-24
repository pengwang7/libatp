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

#ifndef __ATP_LISTENER_H__
#define __ATP_LISTENER_H__

#include <string>
#include <memory>
#include <functional>

#include "net/atp_socket.h"
#include "app/atp_uuid.h"

namespace atp {

class Channel;
class EventLoop;

class Listener : public SocketImpl {
public:
    using NewConnCallbackPtr = std::function<void(int fd, std::string& taddr, void* args)>;

public:
    explicit Listener(EventLoop* event_loop, const std::string& address, unsigned int port);
    ~Listener();

public:
    void listen();

    void accept();

    void stop();

    void setNewConnCallback(NewConnCallbackPtr cb) {
        new_conn_cb_ = cb;
    }

private:
    void acceptHandle();

private:
    EventLoop* event_loop_;
    int listen_fd_;

    struct {
        std::string host_;
        unsigned int port_;
    } address_;

    // The listen fd channel for listener accept.
    std::unique_ptr<Channel> channel_;

    // The variable fn_ will be set by function setNewConnCallback.
    NewConnCallbackPtr new_conn_cb_;
};

} /* end namespace atp */

#endif /* __ATP_LISTENER_H__ */
