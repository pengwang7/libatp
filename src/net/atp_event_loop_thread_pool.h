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

#ifndef __ATP_EVENT_LOOP_THREAD_POOL_H__
#define __ATP_EVENT_LOOP_THREAD_POOL_H__

#include <mutex>
#include <memory>
#include <thread>
#include <vector>

#include "net/atp_state_machine.hpp"

namespace atp {

class EventLoop;

// EventLoopThread and EventLoopThreadPool for TCP server (OLPT mode).
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


class EventLoopPool    : public STATE_MACHINE_INTERFACE {
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
