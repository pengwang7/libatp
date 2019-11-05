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
    Listener* listener = new Listener(event_loop, std::string("0.0.0.0"), 5060);
    listener->setNewConnCallback(&new_conn_handle);
    listener->listenning();
    listener->accept();
    event_loop->dispatch();
    
    atp_logger_close();
    
    return 0;
}
