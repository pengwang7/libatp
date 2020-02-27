#ifndef __ATP_EVENT_LOOP_THREAD_POOL_H__
#define __ATP_EVENT_LOOP_THREAD_POOL_H__

namespace atp {

class EventLoopThreadPool {
public:
    explicit EventLoopThreadPool(size_t thread_num);
    ~EventLoopThreadPool();

public:
    bool start();
    bool stop();
    void join();

    EventLoop* getIOEventLoop();
    size_t getThreadsCount();

private:
    size_t thread_count_;
    std::atomic<size_t> thread_index;
    std::vector<std::shared_ptr<EventLoopThread>> threads_;
};

} /* end namespace atp */

#endif /* __ATP_EVENT_LOOP_THREAD_POOL_H__ */
