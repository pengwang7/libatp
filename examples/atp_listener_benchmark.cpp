#include "glog/logging.h"

#include "atp_buffer.h"
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

struct tslice {
    const char* data;
    size_t size;
};

const char* get_tslice_data(struct tslice* t) {
    return t->data;
}

void test_64() {
    int64_t x = 10;
    int64_t net_x = 0;

    LOG(INFO) << "===: " << htonl(x);
    
    LOG(INFO) << "x: " << x << "   net_x: " << net_x;
    
    net_x = htonll(x);
    LOG(INFO) << "net_x: " << net_x;

    x = ntohll(net_x);
    LOG(INFO) << "x: " << x;

    int64_t u64_host = 10, u64_net;
    int32_t  u32_host_h, u32_host_l;
    u32_host_l = u64_host & 0xffffffff;
    u32_host_h = (u64_host >> 32) & 0xffffffff;
     
    u64_net = htonl(u32_host_l);
    u64_net = ( u64_net << 32 ) | htonl(u32_host_h);

    LOG(INFO) << "=====: " <<u64_net;
}

void test_slice() {
    struct tslice ss;
    ss.data = "ccccccccc";
    
    slice s("wangpeng");

    LOG(INFO) << "====:" << s.data();
    LOG(INFO) << "==11==:" << ss.data;
    LOG(INFO) << "==22==:" << get_tslice_data(&ss);
}

int main() {
    atp_logger_init();

    /* Test slice*/
    test_slice();
    
    /* Test htonll(x) and ntohll(x) */
    test_64();
    return 0;
    
    EventLoop* event_loop = new EventLoop();
    Listener* listener = new Listener(event_loop, std::string("0.0.0.0"), 5060);
    listener->setNewConnCallback(&new_conn_handle);
    listener->listenning();
    listener->accept();
    event_loop->dispatch();
    
    atp_logger_close();
    
    return 0;
}
