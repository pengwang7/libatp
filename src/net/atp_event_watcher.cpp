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

#include <unistd.h>
#include <string.h>

#include "net/atp_config.h"
#include "net/atp_libevent.h"
#include "net/atp_event_loop.h"
#include "net/atp_event_watcher.h"

namespace atp {

EventWatcher::EventWatcher(struct event_base* event_base, DoTasksEventPtr&& handle)
    : event_base_(event_base), event_(NULL), attached_(false), do_tasks_handle(std::move(handle)) {

    event_ = new event;
    memset(event_, 0, sizeof(*event_));
}

EventWatcher::~EventWatcher() {
    detachEventAndFree();
    doTerminate();
}

bool EventWatcher::doInit() {
    if (!doInitImpl()) {
        doTerminate();
        return false;
    }

    event_add(event_, NULL);
    return true;
}

void EventWatcher::doCancel() {
    if (!event_) {
        return;
    }

    detachEventAndFree();

    // If had cancel callback function, do it and clear.
    if (cancel_watcher_handle) {
        cancel_watcher_handle();
        cancel_watcher_handle = DoTasksEventPtr();
    }
}

void EventWatcher::doTerminate() {
    doTerminateImpl();
}

void EventWatcher::detachEventAndFree() {
    if (!event_) {
        return;
    }

    // If is already attached to event_base_, detach from it.
    if (attached_) {
        event_del(event_);
        attached_ = false;
    }

    // In this, do't used event_free to free event_, becase in structure function we used new to alloc memory.
    delete event_;
    event_ = NULL;
}

bool EventWatcher::doWatch(struct timeval* tv) {
    // Add pipe event to libevent event base.
    if (attached_ && event_) {
        if (event_del(event_) != 0) {
            LOG(ERROR) << "[EventWatcher] doWatch delete event from event_base error: fd = " << event_get_fd(event_) << " event = " << event_;
        }
    }

    attached_ = false;

    if (event_add(event_, tv) != 0) {
        LOG(ERROR) << "[EventWatcher] doWatch add event to event_base error: fd = " << event_get_fd(event_) << " event = " << event_;
        return false;
    }

    attached_ = true;

    return true;
}


EventfdWatcher::EventfdWatcher(EventLoop* event_loop, DoTasksEventPtr&& handle, int eventfd_flags)
    : EventWatcher(event_loop->getEventBase(), std::move(handle)) {
    event_fd_ = -1;
    eventfd_flags_ = eventfd_flags;
}

EventfdWatcher::~EventfdWatcher() {
    this->doTerminate();
}

bool EventfdWatcher::asyncWait() {
    return this->doWatch(NULL);
}

void EventfdWatcher::eventNotify() {
    uint64_t data = 3399;
    if (write(event_fd_, &data, sizeof(uint64_t)) != sizeof(uint64_t)) {
        LOG(ERROR) << "[EventfdWatcher] eventNotify error: " << strerror(errno);
        return;
    }

    if (ATP_NET_DEBUG_ON) {
        LOG(INFO) << "[EventfdWatcher] eventNotify success.";
    }
}

void EventfdWatcher::getEventfd(int* fd) {
    *fd = event_fd_;
}

bool EventfdWatcher::doInitImpl() {
    event_fd_ = evutil_make_internal_eventfd(0, eventfd_flags_);
    if (event_fd_ < 0) {
        this->doTerminate();
        return false;
    }

    event_assign(this->event_, this->event_base_, event_fd_, EV_READ | EV_PERSIST,
        &EventfdWatcher::eventfdNotifyHandle, this);

    return true;
}

void EventfdWatcher::doTerminateImpl() {
    close(event_fd_);
    event_fd_ = -1;
    eventfd_flags_ = 0;
}

void EventfdWatcher::eventfdNotifyHandle(int fd, short which, void* args) {
    EventfdWatcher* eventfd_watcher = static_cast<EventfdWatcher*>(args);
    assert(eventfd_watcher);

    uint64_t data;
    if (read(eventfd_watcher->event_fd_, &data, sizeof(uint64_t)) == sizeof(uint64_t)) {
        eventfd_watcher->do_tasks_handle();
        return;
    }
}


PipeEventWatcher::PipeEventWatcher(EventLoop* event_loop, DoTasksEventPtr&& handle)
    : EventWatcher(event_loop->getEventBase(), std::move(handle)) {
    memset(&pipe_fds_, 0, sizeof(pipe_fds_));
}

PipeEventWatcher::~PipeEventWatcher() {
    this->doTerminate();
}

bool PipeEventWatcher::asyncWait() {
    return this->doWatch(NULL);
}

void PipeEventWatcher::eventNotify() {
    char flag = 'c';

    if (write(pipe_fds_[1], &flag, sizeof(flag)) != 1) {
        LOG(ERROR) << "[PipeEventWatcher] eventNotify error: " << strerror(errno);
        return;
    }

    if (ATP_NET_DEBUG_ON) {
        LOG(INFO) << "[PipeEventWatcher] eventNotify success.";
    }
}

void PipeEventWatcher::getPipeInternalReadWriteFd(int* fd0, int* fd1) {
    if (fd0) {
        *fd0 = pipe_fds_[0];
    }

    if (fd1) {
        *fd1 = pipe_fds_[1];
    }
}

bool PipeEventWatcher::doInitImpl() {
    if (evutil_make_internal_pipe(pipe_fds_) < 0) {
        this->doTerminate();
        return false;
    }

    event_assign(this->event_, this->event_base_, pipe_fds_[0], EV_READ | EV_PERSIST,
        &PipeEventWatcher::pipeEventNotifyHandle, this);

    return true;
}

void PipeEventWatcher::doTerminateImpl() {
    close(pipe_fds_[0]);
    close(pipe_fds_[1]);
    memset(pipe_fds_, 0, sizeof(pipe_fds_));
}

void PipeEventWatcher::pipeEventNotifyHandle(int fd, short which, void* args) {
    PipeEventWatcher* pipe_watcher = static_cast<PipeEventWatcher*>(args);
    assert(pipe_watcher);

    char buf[1];
    if (read(pipe_watcher->pipe_fds_[0], buf, sizeof(buf)) == 1) {
        pipe_watcher->do_tasks_handle();
        return;
    }

    LOG(ERROR) << "[PipeEventWatcher] pipeEventNotifyHandle read error: " << strerror(errno);
}


TimerEventWatcher::TimerEventWatcher(EventLoop* event_loop, DoTasksEventPtr&& handle, int delay_second)
    : EventWatcher(event_loop->getEventBase(), std::move(handle)) {
    tv_.tv_sec = delay_second;
    tv_.tv_usec = 0;
}

TimerEventWatcher::~TimerEventWatcher() {
    this->doTerminate();
}

bool TimerEventWatcher::asyncWait() {
    return this->doWatch(&tv_);
}

bool TimerEventWatcher::doInitImpl() {
    event_assign(this->event_, this->event_base_, -1, 0,
        &TimerEventWatcher::timerEventExecuteHandle, this);

    return true;
}

void TimerEventWatcher::doTerminateImpl() {
    memset(&tv_, 0, sizeof(tv_));
}

void TimerEventWatcher::timerEventExecuteHandle(int fd, short which, void* args) {
    TimerEventWatcher* timer_watcher = static_cast<TimerEventWatcher*>(args);
    assert(timer_watcher);

    timer_watcher->do_tasks_handle();
}

} /* end namespace atp */
