#include "atp_event_loop_thread_pool.h"

namespace atp {

EventLoopPool::EventLoopPool(size_t threads_num)
    : threads_num_(threads_num), current_index_(-1) {
    state_.store(STATE_NULL);
}

EventLoopPool::~EventLoopPool() {
    this->state_.store(STATE_STOPPED);
}

bool EventLoopPool::autoStart() {
    if (threads_num_ == 0) {
        this->state_.store(STATE_STOPPED);
        return true;
    }

    this->state_.store(STATE_INIT);
    
    for (size_t i = 0; i < threads_num_; ++ i) {
        std::shared_ptr<EventLoopThread> thd(new EventLoop());
        if (!thd->start) {
            this->state_.store(STATE_STOPPED);
            return false;
        }

        while (!thd->CHECK_STATE(STATE_RUNNING)) {
            sleep(1);
        }

        threads_.push_back(thd);
    }

    this->state_.store(STATE_RUNNING);

    return true;
}

void EventLoopPool::autoJoin() {
    assert(this->CHECK_STATE(STATE_RUNNING));
        
    for (auto& thd : threads_) {
        thd->join();     
    }
}

bool EventLoopPool::autoStop() {
    assert(this->CHECK_STATE(STATE_RUNNING));

    state_.store(STATE_STOPPING);
    
    for (auto& thd : threads_) {
        thd->stop();
    }

    auto stop_fn = [this]() {
        for (auto& thd : this->threads_ ) {
            if (!thd->CHECK_STATE(STATE_STOPPED)) {
                return false;
            }
        }

        return true;
    }

    while (!stop_fn) {
        sleep(1);
    }

    state_.store(STATE_STOPPED);
}

EventLoop* EventLoopPool::getIOEventLoop() {
    assert(this->CHECK_STATE(STATE_RUNNING));

    int current = current_index_.fetch_add(1);
    current = current % threads_.size();

    return threads_[current]->getEventLoop();
}

size_t EventLoopPool::getThreadSize() {
    assert(this->CHECK_STATE(STATE_RUNNING));

    return threads_num_;
}

} /* end namespace atp */
