#ifndef __ATP_LISTENER_H__
#define __ATP_LISTENER_H__

#include <string>
#include <memory>
#include <functional>

#include "atp_debug.h"
#include "atp_uuid.h"
#include "atp_socket.h"

namespace atp {

class Channel;
class EventLoop;

class Listener : public SocketImpl {
public:
    using NewConnCallbackPtr = std::function<void(int fd, std::string& taddr, void* args)>;

public:
    explicit Listener(EventLoop* event_loop, const std::string& address, unsigned int port);
    ~Listener();
    
public:
    void listen();

    void accept();

    void stop();

    void setNewConnCallback(NewConnCallbackPtr cb) {
        new_conn_cb_ = cb;
    }

private:
    void acceptHandle();

private:
    EventLoop* event_loop_;
    int fd_;                                /* Listener file description */                                                                                                                             
                                                                                 
    struct {                                                                                                                           
        std::string host_;                                                       
        unsigned int port_;                                                                                                                           
    } address_;                                                                  
                                                                                 
    std::unique_ptr<Channel> channel_;      /* Listen event channel */                                              
                                                                                              
    NewConnCallbackPtr new_conn_cb_;        /* The variable fn_ will be set by function setNewConnCallback */
};

}/*end namespace atp*/

#endif
