#include "atp_debug.h"
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
    int error = event_base_dispatch(event_base_);
    if (error == 1) {
        LOG(ERROR) << "EventLoop event_base_ no any event register!";
    } else if (error == -1) {
        LOG(ERROR) << "EventLoop event_base_ dispatch failed!";
    } else {
        LOG(INFO) << "EventLoop event_base_ dispatch success!";
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
        return;
    }

    {
        std::lock_guard<std::mutex> lock(mutex_);
        pending_tasks_->emplace_back(task);
    }

    ++ pending_tasks_size_;
    if (!notified_.load()) {
        notified_.store(true);
        event_watcher_.notify();
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
        event_watcher_.notify();
    }
}

void EventLoop::doInit() {
    pending_tasks_ = new std::vector<TaskEventPtr>();
    assert(pending_tasks_ != NULL);

    thread_id_ = std::this_thread::get_id();
    
    doInitPipeEventWatcher();
}

void EventLoop::doInitPipeEventWatcher() {
    event_watcher_.reset(new PipeEventWatcher(this, std::bind(&EventLoop::doPendingTasks, this));
    assert(event_watcher_.doInit());
}

void EventLoop::doPendingTasks() {

}

void EventLoop::stopHandle() {

}

}/*end namespace atp*/

