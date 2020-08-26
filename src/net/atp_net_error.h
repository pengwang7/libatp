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

#ifndef __ATP_NET_ERROR_H__
#define __ATP_NET_ERROR_H__

typedef enum {
    ATP_BUFFER_ERROR_BASE                   =       (-1000),
    ATP_SOCKET_ERROR_BASE                   =       (-2000),
    ATP_CHANNEL_ERROR_BASE                  =       (-3000),
    ATP_TCP_CONN_ERROR_BASE                 =       (-4000),
    ATP_LISTENER_ERROR_BASE                 =       (-5000),
    ATP_EVENT_WATCHER_ERROR_BASE            =       (-6000),
    ATP_EVENT_LOOP_ERROR_BASE               =       (-7000),
    ATP_EVENT_LOOP_THREAD_POOL_ERROR_BASE   =       (-8000),
    ATP_DYNAMIC_THREAD_POOL_ERROR_BASE      =       (-9000),
    ATP_CYCLE_TIMER_ERROR_BASE              =       (-10000),
    ATP_TCP_SERVER_ERROR_BASE               =       (-11000)
} ATP_ERROR_BASE;

#endif /* __ATP_NET_ERROR_H__ */
