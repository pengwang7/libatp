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

#ifndef __ATP_EVENT_WATCHER_H__
#define __ATP_EVENT_WATCHER_H__

#include <functional>

struct event;
struct event_base;

namespace atp {

#define HAVE_EVENTFD
#define HAVE_EVENTPIPE

class EventLoop;

// The EventWatcher is basic class, set structure function is protected destructure function is public.
class EventWatcher {
public:
    using DoTasksEventPtr = std::function<void()>;

protected:
    explicit EventWatcher(struct event_base* event_base, DoTasksEventPtr&& handle);

public:
    ~EventWatcher();

public:
    bool doInit();

    void doCancel();

    void doTerminate();

    void detachEventAndFree();

    bool doWatch(struct timeval* tv);

    void setCancelCallback(DoTasksEventPtr&& cb) {
        cancel_watcher_handle = cb;
    }

protected:
    virtual bool doInitImpl() = 0;
    virtual void doTerminateImpl() {}

protected:
    // The libevent event_base.
    struct event_base* event_base_;

    // The event for pipe read and write.
    struct event* event_;

    // If attached_ is true, the event_ is already attached to event_base_.
    bool attached_;

    DoTasksEventPtr do_tasks_handle;

    DoTasksEventPtr cancel_watcher_handle;
};


// EventfdWatcher is a bridge for multi thread(main event_loop to IO event_loop) message notify(Linux >= 2.6.27).
class EventfdWatcher : public EventWatcher {
public:
	explicit EventfdWatcher(EventLoop* event_loop, DoTasksEventPtr&& handle, int eventfd_flags);
	~EventfdWatcher();

public:
	bool asyncWait();
    void eventNotify();
    void getEventfd(int* fd);

private:
    bool doInitImpl() override;
    void doTerminateImpl() override;
    static void eventfdNotifyHandle(int fd, short which, void* args);

private:
	int event_fd_;
    int eventfd_flags_;
};


// PipeEventWatcher is a bridge for multi thread(main event_loop to IO event_loop) message notify.
class PipeEventWatcher : public EventWatcher {
public:
    explicit PipeEventWatcher(EventLoop* event_loop, DoTasksEventPtr&& handle);
    ~PipeEventWatcher();

public:
    bool asyncWait();
    void eventNotify();
    void getPipeInternalReadWriteFd(int* fd0, int* fd1);

private:
    bool doInitImpl() override;
    void doTerminateImpl() override;
    static void pipeEventNotifyHandle(int fd, short which, void* args);

private:
    int pipe_fds_[2];
};


class TimerEventWatcher : public EventWatcher {
public:
    TimerEventWatcher(EventLoop* event_loop, DoTasksEventPtr&& handle, int delay_second);
    bool asyncWait();

private:
    bool doInitImpl() override;
    void doTerminateImpl() override;
    static void timerEventExecuteHandle(int fd, short which, void* args);

private:
    struct timeval tv_;
};

} /* end namespace atp */

#endif /* __ATP_EVENT_WATCHER_H__ */
