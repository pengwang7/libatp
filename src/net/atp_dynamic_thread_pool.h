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

#ifndef __ATP_DYNAMIC_THREAD_POOL__
#define __ATP_DYNAMIC_THREAD_POOL__

#include <list>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>

#include "net/atp_config.h"

namespace atp {

class BaseThreadPool {
public:
    using TaskPtr = std::function<void()>;

public:
    BaseThreadPool() {}

    ~BaseThreadPool() {}

public:
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
    inline void add(const TaskPtr& callback) override;

    inline size_t getTaskQueueSize() const override;

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
    // Dynamic thread pool state.
    bool shutdown_;

    // Dynamic thread pool minimum thread number.
    size_t core_threads_;

    // Dynamic thread pool current thread number.
    size_t current_threads_;

    // Dynamic thread pool current waiting thread number.
    size_t waiting_threads_;

    // Dynamic thread pool max thread number.
    size_t max_threads_;

    std::mutex lock_;

    std::condition_variable cond_var_;

    std::queue<TaskPtr> callbacks_;

    std::list<DynamicThread*> dead_threads_;
};

} /* end namespace atp */

#endif /* __ATP_DYNAMIC_THREAD_POOL__ */
