namespace atp {

/* The EventWatcher is basic class, set structure function is prcted,ote destructure function is public */
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
    void detachPipeEventAndFree();
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


/* PipeEventWatcher is a bridge for multi thread(main event_loop to IO event_loop) message notify */
class PipeEventWatcher : public EventWatcher {
public:
    explicit PipeEventWatcher();
    ~PipeEventWatcher();
    
public:
    bool asyncWait();
    void eventNotfiy();
    void getPipeInternalReadWriteFd(int* fd0, int* fd1);
    
private:
    bool doInitImpl() override;
    void doTerminateImpl() override;
    static void pipeEventNotifyHandle(int fd, short which, void* args);
    int pipe_fds_[2];
};

/*
class TimerEventWatcher : public EventWatcher {
public:

public:
    
};
*/
}/*end namespace atp*/
