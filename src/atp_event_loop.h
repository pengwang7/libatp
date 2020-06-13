#ifndef __ATP_EVENT_LOOP_H__
#define __ATP_EVENT_LOOP_H__

#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include <atomic>
#include <functional>

#include "atp_event_watcher.h"

struct event;
struct event_base;

namespace atp {

class CycleTimer;

class EventLoop {
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

    bool safety() const {
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
    struct event_base* event_base_; /* The libevent event_base */
    std::mutex mutex_;
    std::thread::id thread_id_;     /* This event_loop in this thread */
#ifdef HAVE_EVENTFD
    std::unique_ptr<EventfdWatcher> event_watcher_;
#else
    std::unique_ptr<PipeEventWatcher> event_watcher_;
#endif
    std::vector<TaskEventPtr>* pending_tasks_;
    std::atomic<int> pending_tasks_size_;
    std::atomic<bool> notified_;
};

}/*end namespace atp*/
#endif
