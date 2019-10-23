#ifndef __ATP_LISTENER_H__
#define __ATP_LISTENER_H__

#include "atp_debug.h"

#include <functional>

struct event;
struct event_base;

namespace atp {

class EventLoop;

class Channel {
public:
    Channel(EventLoop* loop, int fd, bool readable, bool writable);
    ~Channel();

public:
    void attach();
    void detach();
    void update();
    void close();
private:
};

}/*end namespace atp*/

#endif
