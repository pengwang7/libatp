#include "atp_dynamic_thread_pool.h"

namespace atp {

DynamicThreadPool::DynamicThreadPool(size_t core_threads) {
    shutdown_ = false;
    core_threads_ = core_threads;
    current_threads_ = 0;
    waiting_threads_ = 0;
    max_threads_ = THREAD_POOL_MAX_THREADS;

    std::lock_guard<std::mutex> lock(lock_);
    for (size_t i = 0; i < core_threads_; ++ i) {
        current_threads_ ++;
        new DynamicThread(this);
    }
}

DynamicThreadPool::~DynamicThreadPool() {
    std::unique_lock<std::mutex> lock(lock_);
    shutdown_ = true;
    st_cond_.notify_all();

    while (current_threads_ != 0) {
        st_cond_.wait(lock);
    }

    reaper(&dead_threads_);
}

void DynamicThreadPool::add(const TaskPtr& callback) {

}

inline size_t DynamicThreadPool::getTaskQueueSize() const {
    return 0;
}

void DynamicThreadPool::executerImpl() {

}

void DynamicThreadPool::reaper(std::list<DynamicThread*>* tlist) {
    for (auto t = tlist->begin(); t != tlist->end(); t = tlist->erase(t)) {
        delete *t;
    }
}


DynamicThreadPool::DynamicThread::DynamicThread(DynamicThreadPool* pool) {

}

DynamicThreadPool::DynamicThread::~DynamicThread() {
    thd_->join();
    thd_.reset();
}

void DynamicThreadPool::DynamicThread::executer() {
    /* Get a cb from pool tasks and execute it */
    pool_->executerImpl();


    /* Run in this only had two case: 1. shutdown is true, 2. current tasks is empty and waiting threads >= core threads */
    std::lock_guard<std::mutex> lock(lock_);
    pool_->current_threads_ --;
    pool_->dead_threads_.push_back(this);

    /* When destroy pool, last thread must be notify */
    if (pool_->shutdown_ && pool_->current_threads_ == 0) {
        pool_->st_cond_.notify_one();
    }
}

}/*end namespace atp*/
