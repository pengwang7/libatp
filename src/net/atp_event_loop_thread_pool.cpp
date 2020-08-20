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

#include <assert.h>
#include <unistd.h>
#include <functional>

#include "net/atp_event_loop.h"
#include "net/atp_event_loop_thread_pool.h"

namespace atp {

EventLoopThread::EventLoopThread()
    : event_loop_(new EventLoop()) {
    state_.store(STATE_INIT);
}

EventLoopThread::~EventLoopThread() {
    if (CHECK_STATE(STATE_STOPPED)) {
        join();
    }

    state_.store(STATE_NULL);
}

bool EventLoopThread::start() {
    thread_.reset(new std::thread(std::bind(&EventLoopThread::executer, this)));
    if (thread_.get() != nullptr) {
        return true;
    }

    return false;
}

void EventLoopThread::join() {
    std::lock_guard<std::mutex> guard(mutex_);
    if (thread_ && thread_->joinable()) {
        thread_->join();
    }
}

void EventLoopThread::stop() {
    event_loop_->stop();
    state_.store(STATE_STOPPED);
}

EventLoop* EventLoopThread::getEventLoop() const {
    return event_loop_.get();
}

std::thread::id EventLoopThread::getThreadId() const {
    return std::thread::id();
}

void EventLoopThread::executer() {
    state_.store(STATE_RUNNING);

    event_loop_->dispatch();

    state_.store(STATE_STOPPED);
}


EventLoopPool::EventLoopPool(size_t threads_num)
    : threads_num_(threads_num), current_index_(-1) {
    state_.store(STATE_NULL);
}

EventLoopPool::~EventLoopPool() {
    state_.store(STATE_STOPPED);
}

bool EventLoopPool::autoStart() {
    if (threads_num_ == 0) {
        state_.store(STATE_STOPPED);
        return false;
    }

    state_.store(STATE_INIT);

    for (size_t i = 0; i < threads_num_; ++ i) {
        std::shared_ptr<EventLoopThread> thd(new EventLoopThread());
        if (!thd->start()) {
            state_.store(STATE_STOPPED);
            return false;
        }

        while (!thd->CHECK_STATE(STATE_RUNNING)) {
            usleep(5000);
        }

        assert(thd->CHECK_STATE(STATE_RUNNING));
        threads_.push_back(thd);
    }

    state_.store(STATE_RUNNING);

    return true;
}

void EventLoopPool::autoJoin() {
    assert(CHECK_STATE(STATE_STOPPED));

    for (auto& thd : threads_) {
        thd->join();
    }
}

bool EventLoopPool::autoStop() {
    int sleep_seconds = 0;
    assert(CHECK_STATE(STATE_RUNNING));

    state_.store(STATE_STOPPING);

    for (auto& thd : threads_) {
        thd->stop();
    }

    auto stop_fn = [this]() {
        for (auto& thd : this->threads_) {
            if (!thd->CHECK_STATE(STATE_STOPPED)) {
                return false;
            }
        }

        return true;
    };

    // If the thread not change state to STATE_STOPPED after 15s, then thread pool stop failed.
    while (!stop_fn()) {
        usleep(10000);
        ++ sleep_seconds;

        if (sleep_seconds >= 15) {
            return false;
        }
    }

    state_.store(STATE_STOPPED);

    return true;
}

EventLoop* EventLoopPool::getIOEventLoop() {
    assert(CHECK_STATE(STATE_RUNNING));

    int current = current_index_.fetch_add(1);
    current = current % threads_.size();

    return threads_[current]->getEventLoop();
}

size_t EventLoopPool::getThreadSize() {
    assert(CHECK_STATE(STATE_RUNNING));

    return threads_num_;
}

} /* end namespace atp */
