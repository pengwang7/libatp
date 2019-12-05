#ifndef __ATP_DYNAMIC_THREAD_POOL__
#define __ATP_DYNAMIC_THREAD_POOL__

#include <list>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>


#include "atp_debug.h"

namespace atp {

class BaseThreadPool {
public:
    using TaskPtr = std::function<void()>;
    
public:
    BaseThreadPool() {}
    ~BaseThreadPool() {}
    virtual void add(const TaskPtr& callback) = 0;
    virtual size_t getTaskQueueSize() const = 0; 
};

class StaticThreadPool final: public BaseThreadPool {
public:
    StaticThreadPool(size_t total_threads);
    ~StaticThreadPool();
};


class DynamicThreadPool final: public BaseThreadPool {
public:
    explicit DynamicThreadPool(size_t core_threads);
    ~DynamicThreadPool();
    
public:
    void add(const TaskPtr& callback) override;
    size_t getTaskQueueSize() const override;

private:
    class DynamicThread {
        public:
            DynamicThread(DynamicThreadPool* pool);
            ~DynamicThread();

        private:
            void executer();
            
        private:
            DynamicThreadPool* pool_;
            std::unique_ptr<std::thread> thd_;
    };

    void executerImpl();
    void reaper(std::list<DynamicThread*>* tlist);
    
private:
    bool shutdown_;
    size_t core_threads_;                                   /* The dynamic thread pool minimum thread number */
    size_t current_threads_;                                /* The dynamic thread pool current thread number */
    size_t waiting_threads_;                                /* The dynamic thread pool current waiting thread number */
    size_t max_threads_;                                    /* The dynamic thread pool max thread number */

    std::mutex lock_;
    std::condition_variable st_cond_;
    std::queue<TaskPtr> callbacks_;
    std::list<DynamicThread*> dead_threads_;
};

}/*end namespace atp*/

#endif
