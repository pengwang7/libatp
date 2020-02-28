#ifndef __ATP_EVENT_LOOP_THREAD_POOL_H__
#define __ATP_EVENT_LOOP_THREAD_POOL_H__

#include <mutex>
#include <memory>
#include <thread>
#include <vector>

#include "atp_state_machine.h"

namespace atp {

class EventLoop;

/* EventLoopThread and EventLoopThreadPool for TCP server (OLPT mode) */
class EventLoopThread : public STATE_MACHINE_INTERFACE {
public:
    EventLoopThread();
    ~EventLoopThread();
    
public:
    bool start();
    
    void join();
    
    void stop();

    EventLoop* getEventLoop() const;
    
    std::thread::id getThreadId() const;

private:
    void executer();
    
private:
    std::shared_ptr<std::thread> thread_;
    std::shared_ptr<EventLoop> event_loop_;
    std::mutex mutex_;
};


class EventLoopPool             : public STATE_MACHINE_INTERFACE {
public:
    EventLoopPool(size_t threads_num);
    ~EventLoopPool();
    
public:
    bool autoStart();

    bool autoStop();

    void autoJoin();

    EventLoop* getIOEventLoop();

    size_t getThreadSize();
    
private:
    size_t threads_num_;
    std::atomic<int> current_index_;
    std::vector<std::shared_ptr<EventLoopThread>> threads_;
};

} /* end namespace atp */

#endif /* __ATP_EVENT_LOOP_THREAD_POOL_H__ */
