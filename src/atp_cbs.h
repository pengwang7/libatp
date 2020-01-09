#ifndef __ATP_CBS_H__
#define __ATP_CBS_H__

#include <memory>
#include <functional>

namespace atp {

class Buffer;
class Connection;

using SharedConnectionPtr = std::shared_ptr<Connection>;

using ConnectionCallback = std::function<void(const SharedConnectionPtr&)>;
using ReadMessageCallback = std::function<void(const SharedConnectionPtr&, Buffer*)>;
using WriteCompleteCallback = std::function<void(const SharedConnectionPtr&)>;
using TimedoutCallback = std::function<void(const SharedConnectionPtr&)>;
using CloseCallback = std::function<void(const SharedConnectionPtr&)>;

}/*end namespace atp*/

#endif /* __ATP_CBS_H__ */
