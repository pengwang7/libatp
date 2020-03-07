#include "glog/logging.h"

#include "atp_uuid.h"
#include "atp_listener.h"
#include "atp_event_loop.h"
#include "atp_event_loop_thread_pool.h"

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
    LOG(INFO) << "uuid test";

    UUIDGenerator generator;

    LOG(INFO) << generator.generateUUID();
    LOG(INFO) << generator.generateUUID().length();
    atp_logger_close();
    
    return 0;
}

int main02() {
    atp_logger_init();
    {
        const int thd_size = 1;
        EventLoopPool* pool = new EventLoopPool(thd_size);
        assert(pool);

        LOG(INFO) << "main: " << std::this_thread::get_id();
        assert(pool->autoStart());
        assert(pool->autoStop());
        
        LOG(INFO) << "event loop pool stop, wait join thread ...";

        pool->autoJoin();

        delete pool;
    } 
    atp_logger_close();

    return 0;
}

int main01() {
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
