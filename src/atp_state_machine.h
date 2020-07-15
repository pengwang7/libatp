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

#ifndef __ATP_STATE_MACHINE_H__
#define __ATP_STATE_MACHINE_H__

#include <atomic>

namespace atp {

struct STATE_MACHINE_INTERFACE {
public:
    enum SERVICE_STATE {
        STATE_NULL          = 1 << 0,
        STATE_INIT          = 1 << 1,
        STATE_RUNNING       = 1 << 2,
        STATE_STOPPING      = 1 << 3,
        STATE_STOPPED       = 1 << 4,
    };

public:
    bool CHECK_STATE(SERVICE_STATE state) const {
        return state_.load() == state;
    }

    SERVICE_STATE STATE() {
        return state_.load();
    }
    
    std::string STATE_TO_STRING(SERVICE_STATE state) const {
        switch (state) {
            case STATE_NULL:
                return std::string("STATE-NULL");

            case STATE_INIT:
                return std::string("STATE-INIT");

            case STATE_RUNNING:
                return std::string("STATE-RUNNING");

            case STATE_STOPPING:
                return std::string("STATE-STOPPING");

            case STATE_STOPPED:
                return std::string("STATE-STOPPED");

            default:
                return std::string("STATE-INVALID");
        }
    }

protected:
    std::atomic<SERVICE_STATE> state_ = { STATE_NULL };
};

} /* end namespace atp */

#endif /* __ATP_STATE_MACHINE_H__ */
