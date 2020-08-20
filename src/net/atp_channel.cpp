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

#include <string.h>

#include "net/atp_libevent.h"
#include "net/atp_channel.h"
#include "net/atp_event_loop.h"

namespace atp {

static_assert(ATP_READ_EVENT == EV_READ, "CHECK LIBEVENT VERSION FOR EV_READ");
static_assert(ATP_WRITE_EVENT == EV_WRITE, "CHECK LIBEVENT VERSION FOR EV_WRITE");

Channel::Channel(EventLoop* event_loop, int fd, bool readable, bool writable)
    : event_loop_(event_loop), attached_(false) {

    events_ = (readable ? ATP_READ_EVENT : 0) | (writable ? ATP_WRITE_EVENT : 0);

    fd_ = fd;
    assert(fd_ > 0);

    event_ = new(std::nothrow) event;
    assert(event_);

    memset(event_, 0, sizeof(*event_));

    if (ATP_NET_DEBUG_ON) {
        LOG(INFO) << "New channel(" << fd_ << ").";
    }
}

Channel::~Channel() {
    assert(event_ == NULL);

    if (ATP_NET_DEBUG_ON) {
        LOG(INFO) << "Destroy channel(" << fd_ << ").";
    }
}

void Channel::attachToEventLoop() {
    // If the channel events is ATP_NONE_EVENT, don't attached to event loop.
    if (isNone()) {
        return;
    }

    // If the channel attached to event loop. need detach it.
    if (attached_) {
        detachFromEventLoop();
    }

    // Add the event to libevent event base and set event flags is persist.
    event_assign(event_, event_loop_->getEventBase(), fd_,
        events_ | EV_PERSIST, &Channel::eventHandle, this);

    // Add channel success, update state.
    if (!event_add(event_, NULL)) {
        attached_ = true;
    }

    if (ATP_NET_DEBUG_ON) {
        if (attached_) {
            LOG(INFO) << "Attached channel(" << fd_ << ")" << " success.";
        } else {
            LOG(INFO) << "Attached channel(" << fd_ << ")" << " failed.";
        }
    }
}

void Channel::detachFromEventLoop() {
    // Delete event from libevent event base, update state.
    if (attached_ && event_del(event_) == 0) {
        attached_ = false;
    }
}

void Channel::updateEvents() {
    assert(event_loop_->safety());

    if (isNone()) {
        detachFromEventLoop();
    } else {
        attachToEventLoop();
    }
}

void Channel::close() {
    if (event_) {
        detachFromEventLoop();
        delete event_;
        event_ = NULL;
    }

    read_cb_ = EventCallbackPtr();
    write_cb_ = EventCallbackPtr();
}

void Channel::enableEvents(bool readable, bool writable) {
    int orig_events = events_;

    if (readable) {
        events_ |= ATP_READ_EVENT;
    }

    if (writable) {
        events_ |= ATP_WRITE_EVENT;
    }

    if (events_ != orig_events) {
        updateEvents();
    }
}

void Channel::disableEvents(bool readable, bool writable) {
    int orig_events = events_;

    if (readable) {
        events_ &= (~ATP_READ_EVENT);
    }

    if (writable) {
        events_ &= (~ATP_WRITE_EVENT);
    }

    if (events_ != orig_events) {
        updateEvents();
    }
}

void Channel::disableAllEvents() {
    if (events_ != ATP_NONE_EVENT) {
        events_ = ATP_NONE_EVENT;
        updateEvents();
    }
}

int Channel::getInternalFd() const {
    return fd_;
}

std::string Channel::eventsToString() const {
    std::string events_stringify;
    if (events_ & ATP_READ_EVENT) {
        events_stringify += "ATP_READ_EVENT";
    }

    if (events_ & ATP_WRITE_EVENT) {
        events_stringify += "|ATP_WRITE_EVENT";
    }

    return events_stringify;
}

void Channel::eventHandle(int fd, short which) {
    assert(fd == fd_);

    if ((which & ATP_READ_EVENT) && read_cb_) {
        read_cb_();
    }

    if ((which & ATP_WRITE_EVENT) && write_cb_) {
        write_cb_();
    }
}

void Channel::eventHandle(int fd, short which, void* args) {
    Channel* owner = static_cast<Channel*>(args);
    owner->eventHandle(fd, which);
}

} /* end namespace atp */
