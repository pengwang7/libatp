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

#ifndef __ATP_CBS_H__
#define __ATP_CBS_H__

#include <string>
#include <memory>
#include <functional>
#include <unordered_map>

namespace google {

namespace protobuf {

class Message;

} /* end namespace protobuf */

} /* end namespace google */

namespace atp {

class ByteBuffer;
class Connection;
class RpcChannel;
class RpcMessage;

using WeakConnectionPtr = std::weak_ptr<Connection>;
using ConnectionPtr = std::shared_ptr<Connection>;
using RpcChannelPtr = std::shared_ptr<RpcChannel>;
using MessagePtr = std::shared_ptr<::google::protobuf::Message>;
using RpcMessagePtr = std::shared_ptr<RpcMessage>;

using HashTableConn = std::unordered_map<std::string, ConnectionPtr>;

using ConnectionCallback = std::function<void(const ConnectionPtr&)>;
using ReadMessageCallback = std::function<void(const ConnectionPtr&, ByteBuffer&)>;
using WriteCompleteCallback = std::function<void(const ConnectionPtr&)>;
using TimedoutCallback = std::function<void(const ConnectionPtr&)>;
using CloseCallback = std::function<void(const ConnectionPtr&)>;

} /* end namespace atp */

#endif /* __ATP_CBS_H__ */
