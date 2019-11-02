#include <thread>
#include <string>
#include <functional>

struct event;
struct event_base;

namespace atp{

class EventLoop;

class EventLoopThread {
public:

public:
    EventLoopThread();
    ~EventLoopThread();

public:
    bool start();
    void stop();
    void join();
    void setThreadName();
    std::string& getThreadName() const;
    std::thread::id getThreadIdentify() const;
    EventLoop* getInternalIOEventLoop();

private:
};

}/*end namespace atp*/
