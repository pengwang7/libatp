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

#ifndef __ATP_TIMING_WHEEL_HPP__
#define __ATP_TIMING_WHEEL_HPP__

#include <string>
#include <functional>
#include <unordered_set>

#include "net/atp_config.h"
#include "net/atp_cbs.h"
#include "net/atp_tcp_conn.h"
#include "net/atp_ring_buffer.hpp"

namespace atp {

struct Entry;

using WeakEntryPtr = std::weak_ptr<Entry>;

using EntryPtr = std::shared_ptr<Entry>;

using Bucket = std::unordered_set<EntryPtr>;

using CycleBucket = RingBuffer<Bucket>;

#if 0
class test_conn;

using WeakTestConnectionPtr = std::weak_ptr<test_conn>;
using ConnectionTestPtr = std::shared_ptr<test_conn>;

using HashTestTableConn = std::unordered_map<int, ConnectionTestPtr>;

HashTestTableConn test_vec;


class test_conn {
public:
    test_conn(int uuid) {
        uuid_ = uuid;
    }

    ~test_conn() {
        LOG(INFO) << "~test_conn uuid: " << uuid_;
    }

public:
    void close() {
        LOG(INFO) << "connection close uuid: " << uuid_;
        test_vec.erase(uuid_);
    }

    int uuid_;
};

struct Entry {
    Entry(const WeakTestConnectionPtr conn)
        : conn_(conn) {

    }

    ~Entry() {
        ConnectionTestPtr conn = conn_.lock();
        if (conn) {
            conn->close();
        }
    }

    WeakTestConnectionPtr conn_;
};
#endif

struct Entry {
    Entry(const WeakConnectionPtr conn)
        : conn_(conn) {

    }

    ~Entry() {
        ConnectionPtr conn = conn_.lock();
        if (conn) {
            conn->close();
        }

        if (ATP_NET_DEBUG_ON) {
            LOG(INFO) << "The entry destroy.";
        }
    }

    WeakConnectionPtr conn_;
};

/*
 * The module implement a Timing-Wheel,
 * it only used to timeout mechanism for TCP connections,
 * we used it replace one connection with a timer.
 */
class TimingWheel {
public:
    explicit TimingWheel(size_t slot_size, size_t timing_step)
        : slot_size_(slot_size), timing_step_(timing_step) {
        cycle_buckets_.resize(slot_size_);
    }

    ~TimingWheel() {

    }

public:
    void push_back(EntryPtr elem) {
        cycle_buckets_.back().insert(elem);
    }

    void push_bucket(Bucket elem) {
        cycle_buckets_.push_back(elem);
    }

private:
    size_t slot_size_;

    size_t timing_step_;

    CycleBucket cycle_buckets_;
};

} /* end namespace atp */

#endif /* __ATP_TIMING_WHEEL_HPP__ */
