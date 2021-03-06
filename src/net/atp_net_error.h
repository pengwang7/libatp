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

#define ATP_BUFFER_ERROR_BASE                           (-100)

#define ATP_IO_ERROR_BASE                               (ATP_BUFFER_ERROR_BASE - 100)

#define ATP_SOCKET_ERROR_BASE                           (ATP_BUFFER_ERROR_BASE - 200)

#define ATP_CHANNEL_ERROR_BASE                          (ATP_BUFFER_ERROR_BASE - 300)

#define ATP_TCP_CONN_ERROR_BASE                         (ATP_BUFFER_ERROR_BASE - 400)

#define ATP_LISTENER_ERROR_BASE                         (ATP_BUFFER_ERROR_BASE - 500)

#define ATP_TCP_SERVER_ERROR_BASE                       (ATP_BUFFER_ERROR_BASE - 600)

#define ATP_CYCLE_TIMER_ERROR_BASE                      (ATP_BUFFER_ERROR_BASE - 700)

#define ATP_EVENT_LOOP_ERROR_BASE                       (ATP_BUFFER_ERROR_BASE - 800)

#define ATP_EVENT_WATCHER_ERROR_BASE                    (ATP_BUFFER_ERROR_BASE - 900)

#define ATP_DYNAMIC_THREAD_POOL_ERROR_BASE              (ATP_BUFFER_ERROR_BASE - 1000)

#define ATP_EVENT_LOOP_THREAD_POOL_ERROR_BASE           (ATP_BUFFER_ERROR_BASE - 1100)

#endif /* __ATP_NET_ERROR_H__ */
