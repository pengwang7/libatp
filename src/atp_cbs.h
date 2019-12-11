#ifndef __ATP_CBS_H__
#define __ATP_CBS_H__

#include <memory>

namespace atp {

class Connection;

using SharedConnectionPtr = std::shared_ptr<Connection>;

}/*end namespace atp*/

#endif
