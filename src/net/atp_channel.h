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

#ifndef __ATP_CHANNEL_H__
#define __ATP_CHANNEL_H__

#include <functional>

#include "net/atp_config.h"

struct event;
struct event_base;

namespace atp {

class EventLoop;

class Channel {
public:
    using EventCallbackPtr = std::function<void()>;

public:
    Channel(EventLoop* event_loop, int fd, bool readable, bool writable);
    ~Channel();

public:
    void attachToEventLoop();
    void detachFromEventLoop();
    void updateEvents();
    void close();

public:
    void enableEvents(bool readable, bool writable);
    void disableEvents(bool readable, bool writable);
    void disableAllEvents();
    int getInternalFd() const;
    std::string eventsToString() const;

public:
    void setReadCallback(const EventCallbackPtr& cb) {
        read_cb_ = cb;
    }

    void setWriteCallback(const EventCallbackPtr& cb) {
        write_cb_ = cb;
    }

    bool isAttached() const {
        return attached_;
    }

    bool isNone() const {
        return (events_ == ATP_NONE_EVENT);
    }

    bool isReadable() const {
        return (events_ & ATP_READ_EVENT) != 0;
    }

    bool isWritable() const {
        return (events_ & ATP_WRITE_EVENT) != 0;
    }

private:
    // In this, using two event handle to avoid the limitation of static function.
    void eventHandle(int fd, short which);

    static void eventHandle(int fd, short which, void* args);

private:
    // IO event loop.
    EventLoop*       event_loop_;

    // Libevent event for socket read/write.
    struct event*    event_;

    // Socket file description.
    int              fd_;

    // Identify event is read、write、both.
    int              events_;

    // Identify event weather or not attach to libevent event_base.
    bool             attached_;

    // The libevent event read cb.
    EventCallbackPtr read_cb_;

    // The libevent event write cb.
    EventCallbackPtr write_cb_;
};

} /* end namespace atp */

#endif /* __ATP_CHANNEL_H__ */
