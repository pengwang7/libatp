#include "glog/logging.h"

#include "atp_listener.h"
#include "atp_event_loop.h"

using namespace atp;

void atp_logger_init() {
    FLAGS_alsologtostderr = true;                                                
    FLAGS_colorlogtostderr = true;
    FLAGS_logbufsecs = 0;                                                        
    FLAGS_max_log_size = 1800;                                                   
                                                                                 
    google::InitGoogleLogging("test");                                           
    google::SetLogDestination(google::GLOG_INFO,"log-");                         
}

void atp_logger_close() {
    google::ShutdownGoogleLogging();
}

/* The new_conn_handle change fd to conn */
static void new_conn_handle(int fd, const std::string& taddr, void* args) {
    LOG(INFO) << "fd = " << fd << "  remote address = " << taddr;
}

int main() {
    atp_logger_init();

    EventLoop* event_loop = new EventLoop();
    assert(event_loop);
    
    Listener* listener = new Listener(event_loop, std::string("0.0.0.0"), 5060);
    assert(listener);
    
    listener->setNewConnCallback(&new_conn_handle);
    listener->listen();
    listener->accept();
    event_loop->dispatch();

    delete event_loop;
    event_loop = nullptr;
    
    atp_logger_close();
    
    return 0;
}
