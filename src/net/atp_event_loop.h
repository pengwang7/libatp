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

#ifndef __ATP_EVENT_LOOP_H__
#define __ATP_EVENT_LOOP_H__

#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include <atomic>
#include <functional>

#include "net/atp_event_watcher.h"
#include "net/atp_state_machine.hpp"

struct event;
struct event_base;

namespace atp {

class CycleTimer;

class EventLoop final : public STATE_MACHINE_INTERFACE {
public:
    using TaskEventPtr = std::function<void()>;

public:
    EventLoop();

    ~EventLoop();

public:
    void dispatch();

    void stop();

    void sendToQueue(const TaskEventPtr& task);

    void sendToQueue(TaskEventPtr&& task);

    std::shared_ptr<CycleTimer> addCycleTask(int delay_ms, const TaskEventPtr& task, bool persist);

public:
    struct event_base* getEventBase() const {
        return event_base_;
    }

    bool threadSafety() const {
        return thread_id_ == std::this_thread::get_id();
    }

    int pendingTaskQueueSize() const {
        return pending_tasks_size_.load();
    }

    const std::thread::id& getThreadId() {
        return thread_id_;
    }

    int getPendingTaskQueueSize() const {
        return pending_tasks_->size();
    }

    bool pendingTaskQueueIsEmpty() {
        return pending_tasks_->empty();
    }

private:
    void doInit();

    void doInitEventWatcher();

    void doPendingTasks();

    void stopHandle();

private:
    struct event_base* event_base_;

    std::mutex mutex_;

    std::thread::id thread_id_;
#ifdef HAVE_EVENTFD
    std::unique_ptr<EventfdWatcher> event_watcher_;
#else
    std::unique_ptr<PipeEventWatcher> event_watcher_;
#endif

    std::vector<TaskEventPtr>* pending_tasks_;

    std::atomic<int> pending_tasks_size_;

    std::atomic<bool> notified_;
};

} /* end namespace atp */

#endif /* __ATP_EVENT_LOOP_H__ */
