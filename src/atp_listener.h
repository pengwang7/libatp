#ifndef __ATP_LISTENER_H__
#define __ATP_LISTENER_H__

#include <string>
#include <memory>
#include <functional>

#include "atp_debug.h"

namespace atp {

class Channel;
class EventLoop;

class Listener {
public:
    using NewConnCallbackPtr = std::function<void(int fd, const std::string& taddr, void* args)>;

public:
    explicit Listener(EventLoop* event_loop, const std::string& address);
    ~Listener();
    
public:
    void listenning();

    void accept();

    void stop();

    void setNewConnCallback(NewConnCallbackPtr& cb) {
        new_conn_cb_ = cb;
    }

private:
    void accpetHandle();

private:
    EventLoop* event_loop_;
    int fd_;                                /* Listener file description */                                                                                                                             
                                                                                 
    struct {                                                                     
        std::string full_;                                                       
        std::string host_;                                                       
        int port_;                                                               
        int is_ipv6_;                                                            
    } address_;                                                                  
                                                                                 
    std::unique_ptr<Channel> channel_;      /* Listen event channel */                                              
                                                                                              
    NewConnCallbackPtr new_conn_cb_;        /* The variable fn_ will be set by function setNewConnCallback */
};

}/*end namespace atp*/

#endif
