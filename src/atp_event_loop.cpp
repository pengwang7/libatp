#include "glog/logging.h"

#include "atp_debug.h"
#include "atp_timer.h"
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

    /* Last set thread id, event_loop in realy running thread env */
    thread_id_ = std::this_thread::get_id();
    
    int error = event_base_dispatch(event_base_);
    if (error == 1) {
        LOG(ERROR) << "EventLoop event_base_ no any event register!";
    } else if (error == -1) {
        LOG(ERROR) << "EventLoop event_base_ dispatch failed!";
    }
}

/* Can't call stopHandle directly, because you are not sure which thread is calling stop */
void EventLoop::stop() {
    sendToQueue(std::bind(&EventLoop::stopHandle, this));
}

/* 判断任务是否可以在当前前程中执行 如果可以则直接执行 否则 路由到对应的线程进行执行 */
void EventLoop::sendToQueue(const TaskEventPtr& task) {
    if (safety()) {
		if (ATP_DEBUG_ON) {
			LOG(INFO) << "in thread same thread";
		}
        
        /* If in loop thread, execute the task function */
        task();

        return;
    }

	if (ATP_DEBUG_ON) {
		LOG(INFO) << "send to other threads";
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
		if (ATP_DEBUG_ON) {
			LOG(INFO) << "in thread same thread";
		}

        /* If in loop thread, execute the task function */
        task();
        return;
    }

	if (ATP_DEBUG_ON) {
		LOG(INFO) << "send to other threads";
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
    /* First set thread id, When used multi threads mode, */ 
    /* the create event_loop thread and dispatch event_loop thread not in the same thread, */
    /* The first set for listener, because that init used sendToQueue */
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
	/* Why used a tmp_pending_tasks in here? */
	/* 1.This is to prevent call this func blocking by insert new tasks */
	/* 2.Timely release pending_tasks_ accupation memory by std::vector swap method */
	std::vector<TaskEventPtr> tmp_pending_tasks;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        notified_.store(false);
        pending_tasks_->swap(tmp_pending_tasks);
    }

    if (ATP_DEBUG_ON) {
        LOG(INFO) << "The begin current tasks size: " << tmp_pending_tasks.size();
        LOG(INFO) << "Do tasks thread id: " << std::this_thread::get_id();
    }

    /* Execute other threads send to this thread(safety thread) tasks */
    for (size_t i = 0; i < tmp_pending_tasks.size(); ++ i) {
        tmp_pending_tasks[i]();
        -- pending_tasks_size_;

        if (ATP_DEBUG_ON) {
            LOG(INFO) << "Do task, current task size: " << pending_tasks_size_;
        }
    }
}

void EventLoop::stopHandle() {
    /* If event_base_dispatch(base) and event_base_loopexit(base) in two different threads, */
    /* The loop will not exit, So we need to ensure that execute in the same thread */
    auto fn = [this]() {
        while (true) {
            if (pendingTaskQueueIsEmpty()) {
                LOG(INFO) << "The stopHandle tmp_pending_tasks is empty";
                break;
            }

            if (ATP_DEBUG_ON) {
                LOG(INFO) << "Now, doing tasks before event loop stop";
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
