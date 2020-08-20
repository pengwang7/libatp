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
#include "net/atp_libevent.h"
#include "net/atp_cycle_timer.h"
#include "net/atp_event_loop.h"

namespace atp {

EventLoop::EventLoop()
    : pending_tasks_size_(0), notified_(false) {
    // Each event_base executes in a single thread,
    // so select event_base with no locks to reduce the performance cost of event_base underlying locking.
    struct event_config* cfg = event_config_new();
    assert(cfg != NULL);

    event_config_set_flag(cfg, EVENT_BASE_FLAG_NOLOCK);
    event_config_set_flag(cfg, EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST);

    event_base_ = event_base_new_with_config(cfg);
    assert(event_base_ != NULL);

    event_config_free(cfg);
    cfg = NULL;

    doInit();
}

EventLoop::~EventLoop() {
    if (event_base_ != NULL) {
        event_base_free(event_base_);
        event_base_ = NULL;
    }

    if (pending_tasks_ != NULL) {
        delete pending_tasks_;
        pending_tasks_ = NULL;
        pending_tasks_size_ = 0;
    }
}

void EventLoop::dispatch() {
    assert(event_watcher_->asyncWait());

    // Last set thread id
    thread_id_ = std::this_thread::get_id();

    int error = event_base_dispatch(event_base_);
    if (error == 1) {
        LOG(ERROR) << "EventLoop event_base_ no any event register!";
    } else if (error == -1) {
        LOG(ERROR) << "EventLoop event_base_ dispatch failed!";
    }
}

// Don't call this function directly, because not sure which thread is call this function.
void EventLoop::stop() {
    sendToQueue(std::bind(&EventLoop::stopHandle, this));
}

void EventLoop::sendToQueue(const TaskEventPtr& task) {
    if (safety()) {

        task();

        return;
    }

    {
        std::lock_guard<std::mutex> lock(mutex_);
        pending_tasks_->emplace_back(task);
    }

    ++ pending_tasks_size_;
    if (!notified_.load()) {
        notified_.store(true);
        event_watcher_->eventNotify();
    }
}

void EventLoop::sendToQueue(TaskEventPtr&& task) {
    if (safety()) {

        task();

        return;
    }

    {
        std::lock_guard<std::mutex> lock(mutex_);
        pending_tasks_->emplace_back(std::move(task));
    }

    ++ pending_tasks_size_;
    if (!notified_.load()) {
        notified_.store(true);
        event_watcher_->eventNotify();
    }
}

std::shared_ptr<CycleTimer> EventLoop::addCycleTask(int delay_ms, const TaskEventPtr& task, bool persist) {
    std::shared_ptr<CycleTimer> cycle_timer = CycleTimer::newCycleTimer(this, delay_ms, task, persist);
    cycle_timer->start();

    return cycle_timer;
}

void EventLoop::doInit() {
    // First set thread id, When used multi threads mode,
    // the create event_loop thread and dispatch event_loop thread not in the same thread,
    // The first set for listener, because that init used sendToQueue.
    thread_id_ = std::this_thread::get_id();
    pending_tasks_ = new std::vector<TaskEventPtr>();
    assert(pending_tasks_ != NULL);

    doInitEventWatcher();
}

void EventLoop::doInitEventWatcher() {
#ifdef HAVE_EVENTFD
    event_watcher_.reset(new EventfdWatcher(this, std::bind(&EventLoop::doPendingTasks, this), 0));
#else
    event_watcher_.reset(new PipeEventWatcher(this, std::bind(&EventLoop::doPendingTasks, this)));
#endif
    assert(event_watcher_->doInit());
}

void EventLoop::doPendingTasks() {
	// Why used a tmp_pending_tasks in here?
	// 1.This is to prevent call this func blocking by insert new tasks.
	// 2.Timely release pending_tasks_ accupation memory by std::vector swap method.
	std::vector<TaskEventPtr> tmp_pending_tasks;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        notified_.store(false);
        pending_tasks_->swap(tmp_pending_tasks);
    }

    for (size_t i = 0; i < tmp_pending_tasks.size(); ++ i) {
        tmp_pending_tasks[i]();
        -- pending_tasks_size_;
    }
}

void EventLoop::stopHandle() {
    // If event_base_dispatch(base) and event_base_loopexit(base) in two different threads,
    // The loop will not exit, So we need to ensure that execute in the same thread.
    auto fn = [this]() {
        while (true) {
            if (pendingTaskQueueIsEmpty()) {
                LOG(INFO) << "The stopHandle tmp_pending_tasks is empty";
                break;
            }

            doPendingTasks();
        }
    };

    fn();

    event_base_loopexit(event_base_, NULL);

    if (!pendingTaskQueueIsEmpty()) {
        LOG(INFO) << "After event loop stopped, the tasks size: " << getPendingTaskQueueSize();

        std::vector<TaskEventPtr>().swap(*pending_tasks_);
    }
}

} /* end namespace atp */
