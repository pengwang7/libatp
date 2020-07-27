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

#ifndef __ATP_RING_BUFFER_HPP__
#define __ATP_RING_BUFFER_HPP__

#include <list>

#include "glog/logging.h"

namespace atp {

const size_t DEFAULT_BUFF_SIZE = 300;

template <class T>
class RingBuffer {
public:
    RingBuffer() {
        max_size_ = DEFAULT_BUFF_SIZE;
    }

    explicit RingBuffer(size_t size) {
        max_size_ = size;
    }

    ~RingBuffer() {
        core_list_.clear();
        max_size_ = 0;
    }

public:
    void push_back(T elem) {
        if (core_list_.size() == max_size_) {
            core_list_.pop_front();
        }

        core_list_.push_back(elem);
    }

    int resize(size_t size) {
        max_size_ = size;

        return 0;
    }

    bool empty() { return core_list_.empty(); }

    bool full() { return core_list_.size() == max_size_; }

    size_t size() { return core_list_.size(); }

    size_t length() { return core_list_.size(); }

    T& back() { return core_list_.back(); }

private:
    size_t max_size_;
    std::list<T> core_list_;
};

} /* end namespace atp */

#endif /* __ATP_RING_BUFFER_HPP__ */
