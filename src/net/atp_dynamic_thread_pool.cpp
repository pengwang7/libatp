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

#include "net/atp_dynamic_thread_pool.h"

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
    cond_var_.notify_all();

    while (current_threads_ != 0) {
        cond_var_.wait(lock);
    }

    reaper(&dead_threads_);
}

inline void DynamicThreadPool::add(const TaskPtr& callback) {
    std::lock_guard<std::mutex> lock(lock_);
    callbacks_.push(callback);

    // 1.waiting_threads_ is 0 and current_threads_ >= max_threads_, wait a idle thread from pool do task.
    // 2.waiting_threads_ is 0 and current_threads_ < max_threads_, create a new thread do task.
    // 3.waiting_threads_ is not 0, notify pool thread do task.
    if (waiting_threads_ == 0 && current_threads_ >= max_threads_) {
        return;
    } else if (waiting_threads_ == 0) {
        ++ current_threads_;
        new DynamicThread(this);
    } else {
        cond_var_.notify_one();
    }
}

inline size_t DynamicThreadPool::getTaskQueueSize() const {
    return callbacks_.size();
}

void DynamicThreadPool::executerImpl() {
    for (; ;) {
        std::unique_lock<std::mutex> lock(lock_);
        if (!shutdown_ && callbacks_.empty()) {
            if (waiting_threads_ >= core_threads_) {
                lock.unlock();
                return;
            }

            ++ waiting_threads_;
            cond_var_.wait(lock);
            -- waiting_threads_;
        }

        if (!callbacks_.empty()) {
            auto cb = callbacks_.front();
            callbacks_.pop();
            lock.unlock();
            cb();
        } else if (shutdown_) {
            break;
        }
    }
}

void DynamicThreadPool::reaper(std::list<DynamicThread*>* tlist) {
    for (auto t = tlist->begin(); t != tlist->end(); t = tlist->erase(t)) {
        delete (*t);
    }
}


DynamicThreadPool::DynamicThread::DynamicThread(DynamicThreadPool* pool)
    : pool_(pool), thd_(new std::thread(&DynamicThreadPool::DynamicThread::executer, this)) {

}

DynamicThreadPool::DynamicThread::~DynamicThread() {
    thd_->join();
    thd_.reset();
}

void DynamicThreadPool::DynamicThread::executer() {
    // Get a cb from pool tasks and execute it.
    pool_->executerImpl();

    // Run in this only had two case:
    // 1. shutdown is true.
    // 2. current tasks is empty and waiting threads >= core threads.
    std::lock_guard<std::mutex> lock(pool_->lock_);
    pool_->current_threads_ --;
    pool_->dead_threads_.push_back(this);

    // When destroy pool, last thread must be notify.
    if (pool_->shutdown_ && pool_->current_threads_ == 0) {
        pool_->cond_var_.notify_one();
    }
}

} /* end namespace atp */
