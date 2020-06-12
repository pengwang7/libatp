#ifndef __ATP_TIMER_H__
#define __ATP_TIMER_H__

#include <memory>
#include <functional>

namespace atp {

class EventLoop;
class TimerEventWatcher;

class CycleTimer : public std::enable_shared_from_this<CycleTimer> {
public:
    typedef std::function<void()> ExpiresFunctor;

public:
    static std::shared_ptr<CycleTimer> newCycleTimer(EventLoop* loop, int delay_second, const ExpiresFunctor& cb, bool persist);

    static std::shared_ptr<CycleTimer> newCycleTimer(EventLoop* loop, int delay_second, ExpiresFunctor&& cb, bool persist);

    void start();

    void cancel();

    void setCancelCallback(const ExpiresFunctor& cb) {
        cancel_fn_ = cb;
    }

public:
    ~CycleTimer();

private:
    CycleTimer(EventLoop* loop, int delay_second, const ExpiresFunctor& cb, bool persist);

    CycleTimer(EventLoop* loop, int delay_second, ExpiresFunctor&& cb, bool persist);

    void onTrigger();

    void onCancel();

private:
    EventLoop* loop_;
    std::shared_ptr<TimerEventWatcher> timer_watcher_;
    std::shared_ptr<CycleTimer> self_; // Hold myself
    ExpiresFunctor expires_fn_;
    int delay_second_;
    bool persist_;
    ExpiresFunctor cancel_fn_;
};

} /* end namespace atp */

#endif /* __ATP_TIMER_H__ */
