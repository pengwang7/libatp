#include "glog/logging.h"

#include "atp_debug.h"
#include "atp_libevent.h"
#include "atp_event_loop.h"

namespace atp {

/* 每个event_base 都单独在一个线程内执行 所以这里选择不带有锁的event_base */ 
/* 来减少event_base底层加锁带来的性能消耗 */
EventLoop::EventLoop()
    : pending_tasks_size_(0), notified_(false) {
    /* Create libevent event_base and set options */
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
    
    int error = event_base_dispatch(event_base_);
    if (error == 1) {
        LOG(ERROR) << "EventLoop event_base_ no any event register!";
    } else if (error == -1) {
        LOG(ERROR) << "EventLoop event_base_ dispatch failed!";
    }
}

/* 不可以直接调用stopHandle, 因为我们不确定调用stop的线程是哪一个 */
void EventLoop::stop() {
    sendToQueue(std::bind(&EventLoop::stopHandle, this));
}

/* 判断任务是否可以在当前前程中执行 如果可以则直接执行 否则 路由到对应的线程进行执行 */
void EventLoop::sendToQueue(const TaskEventPtr& task) {
    if (safety()) {
        /* If in loop thread, execute the task function */
        task();

        if (ATP_DEBUG_ON) {
            LOG(INFO) << "sendToQueue process a task";
        }
        
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
        /* If in loop thread, execute the task function */
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

void EventLoop::doInit() {
    pending_tasks_ = new std::vector<TaskEventPtr>();
    assert(pending_tasks_ != NULL);

    thread_id_ = std::this_thread::get_id();

    doInitEventWatcher();
}

void EventLoop::doInitEventWatcher() {
#ifdef HAVE_EVENTFD
    event_watcher_.reset(new EventfdWatcher(this, std::bind(&EventLoop::doPendingTasks, this)), EFD_NONBLOCK | EFD_CLOEXEC);  
#else
    event_watcher_.reset(new PipeEventWatcher(this, std::bind(&EventLoop::doPendingTasks, this)));  
#endif
    assert(event_watcher_->doInit());
}

void EventLoop::doPendingTasks() {
    /* Timely release pending_tasks_ accupation memory by std::vector swap method, */
    /* tmp_pending_tasks will be destruction after this function */
    std::vector<TaskEventPtr> tmp_pending_tasks;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        notified_.store(false);
        pending_tasks_->swap(tmp_pending_tasks);
    }

    if (ATP_DEBUG_ON) {
        LOG(INFO) << "The current tasks size: " << tmp_pending_tasks.size();
    }

    /* Execute other threads send to this thread(safety thread) tasks */
    for (size_t i = 0; i < tmp_pending_tasks.size(); ++ i) {
        tmp_pending_tasks[i]();
        -- pending_tasks_size_;
    }
}

void EventLoop::stopHandle() {
    auto fn = [this]() {
        while (true) {
            if (pendingTaskQueueIsEmpty()) {
                LOG(INFO) << "The stopHandle tmp_pending_tasks is empty";
            }

            if (ATP_DEBUG_ON) {
                LOG(INFO) << "Now, doing tasks before event loop stop";
            }
            
            doPendingTasks();
        }
    };

    fn();

    event_base_loopexit(event_base_, NULL);

    LOG(INFO) << "The event loop is stopped";
    
    if (!pendingTaskQueueIsEmpty()) {
        LOG(INFO) << "After event loop stopped, the tasks size: " << getPendingTaskQueueSize();
        std::vector<TaskEventPtr>().swap(*pending_tasks_);
    }
}

}/*end namespace atp*/
