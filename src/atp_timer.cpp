#include "atp_timer.h"
#include "atp_event_loop.h"

namespace atp {

std::shared_ptr<CycleTimer> CycleTimer::newCycleTimer(EventLoop* loop, int delay_second, const ExpiresFunctor& cb, bool persist) {
    std::shared_ptr<CycleTimer> timer(new CycleTimer(loop, delay_second, cb, persist));
    timer->self_ = timer;

    return timer;
}

std::shared_ptr<CycleTimer> CycleTimer::newCycleTimer(EventLoop* loop, int delay_second, ExpiresFunctor&& cb, bool persist) {
    std::shared_ptr<CycleTimer> timer(new CycleTimer(loop, delay_second, cb, persist));
    timer->self_ = timer;

    return timer;
}

void CycleTimer::start() {
    auto fn = [this]() {
        // The std::bind with shared_from_this() will add one to shared_ptr reference
        // So, in this, the self_.use_count() is three(hisself, and two std::bind)

        timer_watcher_.reset(new TimerEventWatcher(loop_, std::bind(&CycleTimer::onTrigger, shared_from_this()), delay_second_));
        timer_watcher_->setCancelCallback(std::bind(&CycleTimer::onCancel, shared_from_this()));
        timer_watcher_->doInit();
        timer_watcher_->asyncWait();
    };

    loop_->sendToQueue(std::move(fn));
}

void CycleTimer::cancel() {
    if (timer_watcher_) {
        loop_->sendToQueue(std::bind(&TimerEventWatcher::doCancel, timer_watcher_));
    }
}

CycleTimer::~CycleTimer() {

}

CycleTimer::CycleTimer(EventLoop* loop, int delay_second, const ExpiresFunctor& cb, bool persist)
        : loop_(loop), expires_fn_(cb), delay_second_(delay_second), persist_(persist) {

}

CycleTimer::CycleTimer(EventLoop* loop, int delay_second, ExpiresFunctor&& cb, bool persist)
        : loop_(loop), expires_fn_(cb), delay_second_(delay_second), persist_(persist) {

}

void CycleTimer::onTrigger() {
    if (expires_fn_) {
        expires_fn_();
    }

    if (persist_) {
        timer_watcher_->asyncWait();
    } else {
        expires_fn_ = ExpiresFunctor();
        timer_watcher_.reset();
        self_.reset();
    }    
}

void CycleTimer::onCancel() {
    persist_ = false;

    if (cancel_fn_) {
        cancel_fn_();
        cancel_fn_ = ExpiresFunctor();
    }

    expires_fn_ = ExpiresFunctor();
    timer_watcher_.reset();
    self_.reset();
}

} /* end namespace atp */
