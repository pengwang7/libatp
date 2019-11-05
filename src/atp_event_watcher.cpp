#include "atp_event_watcher.h"

namespace atp {

EventWatcher::EventWatcher(struct event_base* event_base, DoTasksEventPtr&& handle)
    event_base_(event_base), event_(NULL), attached_(false), do_tasks_handle(std::move(handle)) {

    event_ = new event;
    memset(event_, 0, sizeof(*event_));
}

EventWatcher::~EventWatcher() {
    detachPipeEventAndFree();
    doTerminate();
}

bool EventWatcher::  doInit() {
    if (!doInitImpl()) {
        doTerminate();
        return false;
    }
    
    event_add(event_, NULL);
    return true;
}

void EventWatcher::  doCancel() {
    if (!event_) {
        return;
    }
    
    detachPipeEventAndFree();

    /* If had cancel callback function, do it and clear */
    if (cancel_watcher_handle) {
        cancel_watcher_handle();
        cancel_watcher_handle = DoTasksEventPtr();
    }
}

void EventWatcher::  doTerminate() {
    doTerminateImpl();
}

void EventWatcher::detachPipeEventAndFree() {
    if (!event_) {
        return;
    }

    /* If is already attached to event_base_, detach from it */
    if (attached_) {
        event_del(event_);
        attached_ = false;
    }

    /* In this, do't used event_free to free event_, becase in structure function we used new to alloc memory */
    delete event_;
    event_ = NULL;
}

bool EventWatcher::doWatch(struct timeval* tv) {
    /* Add pipe event to libevent event base */
    if (attached_ && event_) {
        if (event_del(event_) != 0) {
            LOG(ERROR) << "Delete event from event_base error: fd = " << event_get_fd(event_) << " event = " << event_;
        }
    }

    attached_ = false;

    if (event_add(event_, tv) != 0) {
        LOG(ERROR) << "Add event to event_base error: fd = " << event_get_fd(event_) << " event = " << event_;
        return false;
    }
    
    attached_ = true;

    return true;
}


PipeEventWatcher::PipeEventWatcher(EventLoop* event_loop, DoTasksEventPtr&& handle)
    : EventWatcher(event_loop->getEventBase(), std::move(handle)) {
    memset(&pipe_fds_, 0, sizeof(pipe_fds_));
}

PipeEventWatcher::~PipeEventWatcher() {
    this->doTerminate();
}

bool PipeEventWatcher::asyncWait() {
    /* Never timedout */
    return this->doWatch(NULL);
}

void PipeEventWatcher::eventNotfiy() {
    char flag = 'c';

    if (write(pipe_fds_[1], &flag, sizeof(flag)) != 1) {
        LOG(ERROR) << "Event notify write error: " << strerror(errno);
        return;
    }

    if (ATP_DEBUG_ON) {
        LOG(INFO) << "The event notify success!";
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
    close(pipe_fds[0]);
    close(pipe_fds[1]);
    memset(pipe_fds_, 0, sizeof(pipe_fds_));
}

void PipeEventWatcher::pipeEventNotifyHandle(int fd, short which, void* args) {

}

}/* end namespace atp*/
