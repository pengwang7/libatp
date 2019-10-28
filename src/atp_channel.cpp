#include <assert.h>

#include "atp_channel.h"

namespace atp {

/* Check the libatp events */
static_assert(ATP_READ_EVENT == EV_READ);
static_assert(ATP_WRITE_EVENT == EV_WRITE);

Channel::Channel(EventLoop* event_loop, int fd, bool readable, bool writable) {
    event_loop_ = event_loop;
    events_ = (readable ? ATP_READ_EVENT : 0) | (writable ? ATP_WRITE_EVENT : 0);

    fd_ = fd;
    assert(fd > 0);

    event_ = new(std::nothrow) event;
    assert(event_);

    memset(event_, 0, sizeof(*event_));
}

Channel::~Channel() {
    assert(event_ == NULL);
}

void Channel::attachToEventLoop() {
    /* The channel is none events, not attach to event loop */
    if (isNone()) {
        return;
    }

    /* The channel if already attached, we detach from event loop */
    if (attached_) {
        detachFromEventLoop();
    }

    /* Add event_ default is persist event */
    event_assign(event_, event_loop_->getEventBase(), fd_, 
        events_ | EV_PERSIST, &Channel::eventHandle, this);

    if (!event_add(event_, NULL)) {
        attached_ = true;
    }
}

void Channel::detachFromEventLoop() {
    /* Delete event_ from the libevent event_base */
    if (attached_ && event_del(event_) == 0) {
        attached_ = false;
    }
}

void Channel::updateEvents() {
    assert(event_loop_->threadContext());

    /* Update the events to none/read/write */
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

}/*end namespace atp*/
