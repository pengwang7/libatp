#ifndef __SERVICE_H__
#define __SERVICE_H__

namespace atp {

class Service {
public:
    explicit Service();
    ~Service();

public:
    bool start();
    void stop();

public:
    void setConnectionCallback(const ConnectionCallback& fn) {
        conn_fn_ = fn;        
    }

    void setMessageCallback(const ReadMessageCallback& fn) {
        message_fn_ = fn;
    }

private:
    void startEventLoopPool();
    void stopEventLoopPool();
    void handleNewConnection();
    EventLoop* getIOEventLoop();

private:
    EventLoop* control_event_loop_;
    const std::string baddr_;
    const std::string service_name_;
    std::unique_ptr<Listener> listener_;
    std::unique_ptr<EventLoopThreadPool> event_loop_thread_pool_;
    std::unique_ptr<DynamicThreadPool> dynamic_thread_pool_;

    ConnectionCallback conn_fn_;
    ReadMessageCallback message_fn_;
    size_t thread_num_;
};

} /* end namespace atp */;

#endif /* __SERVICE_H__ */
