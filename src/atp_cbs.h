#ifndef __ATP_CBS_H__
#define __ATP_CBS_H__

#include <string>
#include <memory>
#include <functional>
#include <unordered_map>

namespace atp {

class ByteBuffer;
class Connection;

using ConnectionPtr = std::shared_ptr<Connection>;
using HashTableConn = std::unordered_map<std::string, ConnectionPtr>;

using ConnectionCallback = std::function<void(const ConnectionPtr&)>;
using ReadMessageCallback = std::function<void(const ConnectionPtr&, ByteBuffer&)>;
using WriteCompleteCallback = std::function<void(const ConnectionPtr&)>;
using TimedoutCallback = std::function<void(const ConnectionPtr&)>;
using CloseCallback = std::function<void(const ConnectionPtr&)>;

}/*end namespace atp*/

#endif /* __ATP_CBS_H__ */
