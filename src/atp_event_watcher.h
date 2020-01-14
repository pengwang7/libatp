#ifndef __ATP_EVENT_WATCHER_H__
#define __ATP_EVENT_WATCHER_H__

#include <functional>

struct event;
struct event_base;

namespace atp {

#define HAVE_EVENTFD
#define HAVE_EVENTPIPE

class EventLoop;

/* The EventWatcher is basic class, set structure function is protected destructure function is public */
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

protected:
    /* The derived class must impl */
    virtual bool doInitImpl() = 0;
    virtual void doTerminateImpl() {}

protected:
    struct event_base* event_base_;         /* The libevent event_base */
    struct event* event_;                   /* The event for pipe read and write */
    bool attached_;                         /* If attached_ is true, the event_ is already attached to event_base_ */
    DoTasksEventPtr do_tasks_handle;
    DoTasksEventPtr cancel_watcher_handle;
};


/* EventfdWatcher is a bridge for multi thread(main event_loop to IO event_loop) message notify(Linux >= 2.6.27) */
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
	int event_fd_;
    int eventfd_flags_;
};


/* PipeEventWatcher is a bridge for multi thread(main event_loop to IO event_loop) message notify */
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
    struct timeval tv_;
};

}/*end namespace atp*/
#endif
