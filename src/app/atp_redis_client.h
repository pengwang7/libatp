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

#ifndef __ATP_REDIS_CLIENT_H__
#define __ATP_REDIS_CLIENT_H__

namespace atp {
/* The hiredis interface(sync) for C++ */

typedef struct {
    std::string addr_;
    unsigned int port_;
    std::string passwd_;
} RedisServerInfo;

class RedisClientImpl {
public:
    explicit RedisClientImpl(RedisServerInfo server_info);
    ~RedisClientImpl();

public:
    void redisConnect();
    void redisHeartBeat();
    void redisLibeventAttach();
    void redisAsyncSetConnectCallback();
    void redisAsyncSetDisconnectionCallback();
    void redisExecuteCommandCallback();
};

} /* end namespace atp */
#endif /* __ATP_REDIS_CLIENT_H__ */
