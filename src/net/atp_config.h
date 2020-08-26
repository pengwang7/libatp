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

#ifndef __ATP_CONFIG_H__
#define __ATP_CONFIG_H__

#include <assert.h>

#include "glog/logging.h"

namespace atp {

// Open the libatp net layer log.
#define ATP_NET_DEBUG_ON               (1)


// The libatp none/read/write event flags.
#define ATP_NONE_EVENT                 (0x00)

#define ATP_READ_EVENT                 (0x02)

#define ATP_WRITE_EVENT                (0x04)


// System listen queue max size.
#define ATP_SO_MAX_CONN                (4096)


// ByteBuffer initial prepend size.
#define RESERVED_PREPEND_SIZE          (8)


// ByteBuffer initial body size.
#define INIT_BUFFER_SIZE               (1024)


// RingBuffer initial size.
#define INIT_RING_BUFFER_SIZE          (300)


// Dynamic thread pool max threads.
#define THREAD_POOL_MAX_THREADS        (128)


// Tcp connection read/write timeout ms.
#define CONN_READ_WRITE_EXPIRES        (300)


// Socket retriable error.
#define RETRIABLE_ERROR                (-11)


// Byte order conversion
#define htonll(x)                                \
    ((((x) & 0xff00000000000000ull) >> 56)       \
    | (((x) & 0x00ff000000000000ull) >> 40)      \
    | (((x) & 0x0000ff0000000000ull) >> 24)      \
    | (((x) & 0x000000ff00000000ull) >> 8)       \
    | (((x) & 0x00000000ff000000ull) << 8)       \
    | (((x) & 0x0000000000ff0000ull) << 24)      \
    | (((x) & 0x000000000000ff00ull) << 40)      \
    | (((x) & 0x00000000000000ffull) << 56))

#define ntohll(x) htonll(x)

} /* end namespace atp */

#endif /* __ATP_CONFIG_H__ */
