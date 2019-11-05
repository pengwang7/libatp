namespace atp {

class EventLoop {
public:
    using TaskEventPtr = std::function<void()>;
    
public:
    explicit EventLoop();
    ~EventLoop();

public:
    void dispatch();
    void stop();
    void sendToQueue(const TaskEventPtr& task);
    void sendToQueue(TaskEventPtr&& task);

public:
    srtuct event_base* getEventBase() const {
        return event_base_;
    }

    bool safety() const {
        thread_id_ == std::this_thread::get_id();
    }

    int pendingTaskQueueSize() const {
        return pending_tasks_size_.load();
    }

    const std::thread::id& GetThreadId() {
        return thread_id_;
    }

private:
    void doInit();
    void doInitPipeEventWatcher();
    void doPendingTasks();
    void stopHandle();
    
    int getPendingTaskQueueSize() const {
        return pending_tasks_->size();
    }
    
    bool pendingTaskQueueIsEmpty() {
        return pending_tasks_->empty();
    }
    
private:
    struct event_base* event_base_; /* The libevent event_base */
    std::mutex mutex_;
    std::thread::id thread_id_;     /* This event_loop in this thread */
    std::unique_ptr<PipeEventWatcher> event_watcher_;
    std::vector<TaskEventPtr>* pending_tasks_;
    std::atomic<int> pending_tasks_size_;
    std::atomic<bool> notified_;
};

}/*end namespace atp*/
