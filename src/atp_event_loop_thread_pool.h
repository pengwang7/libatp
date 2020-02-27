#ifndef __ATP_EVENT_LOOP_THREAD_POOL_H__
#define __ATP_EVENT_LOOP_THREAD_POOL_H__

#include "atp_state_machine.h"

namespace atp {

class EventLoopThread : public STATE_MACHINE_INTERFACE {

};


class EventLoopPool           : public STATE_MACHINE_INTERFACE {
public:
    EventLoopPool(size_t threads_num);
    ~EventLoopPool();
    
public:
    bool autoStart();

    bool autoStop();

    bool autoJoin();

    EventLoop* getIOEventLoop();

    size_t getThreadSize();
    
private:
    size_t threads_num_;
    std::atomic<int> current_index_;
    std::vector<std::shared_ptr<EventLoopThread>> threads_;
};

} /* end namespace atp */

#endif /* __ATP_EVENT_LOOP_THREAD_POOL_H__ */
