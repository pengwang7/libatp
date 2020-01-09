#include "glog/logging.h"

#include "atp_buffer.h"
#include "atp_listener.h"
#include "atp_event_loop.h"
#include "atp_timing_cycle.h"

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

void test_array() {
    LOG(INFO) << "test_begin=====";

    const int size = 10;
    std::array<std::unique_ptr<int>, size> p;
    for (int i = 0; i < size; ++ i) {
        std::unique_ptr<int> a(new int (i));
        p[i] = std::move(a);
    }

    for (int i = 0; i < size; ++ i) {
        LOG(INFO) << "i: " << *p[i];
    }
    
    LOG(INFO) << "test_end=====";
}

void test_cycle() {
    CircularBuffer<int> cycle(3);
    cycle.push_back(1);
    cycle.push_back(2);
    cycle.push_back(3);

    cycle.traverse();

    cycle.push_back(4);
    cycle.push_back(5);

    cycle.traverse();

    cycle.push_back(6);

    cycle.traverse();
}

#if 0
void test_on_timer(void* args) {
	TimingWheel* timing = static_cast<TimingWheel*>(args);
	
	while (true) {
		timing->cycle_buckets_.push_back(Bucket());
		LOG(INFO) << "1 push a bucket!!!";
		sleep(1);
	}
}



void test_on_conn(void* args) {
	TimingWheel* timing = static_cast<TimingWheel*>(args);
	int i = 0;
	while (true) {
		
		std::shared_ptr<Connection> conn(new Connection(++ i));
		vec.push_back(conn);
		SharedEntryPtr entry(new Entry(conn));
		timing->push_back(entry);
		LOG(INFO) << "2 push connection!!! id: " << i << "  time: " << time(nullptr);

		sleep(2);
	}
}

void test_timing_wheel() {
	TimingWheel timing_wheel(10, 1, "");
	std::unique_ptr<std::thread> thd(new std::thread(test_on_timer, &timing_wheel));
	std::unique_ptr<std::thread> thd2(new std::thread(test_on_conn, &timing_wheel));

	thd->join();
	thd2->join();
//	std::thread(test_on_timer, nullptr);


/*
    LOG(INFO) << timing_wheel.name();
    
    for (; ;) {
        sleep(1);

        LOG(INFO) << "add new conn";
    
    }
    */
}
#endif
int main() {
    atp_logger_init();

    /* Test timing wheel */
    //test_timing_wheel();

	//return 0;
	
    /* Test circual buffer */
    test_cycle();
    
    /* Test array */
    test_array();

    return 0;
    
    /* Test slice*/
    test_slice();
    
    /* Test htonll(x) and ntohll(x) */
    test_64();
    return 0;
    
    EventLoop* event_loop = new EventLoop();
    Listener* listener = new Listener(event_loop, std::string("0.0.0.0"), 5060);
    listener->setNewConnCallback(&new_conn_handle);
    listener->listening();
    listener->accept();
    event_loop->dispatch();
    
    atp_logger_close();
    
    return 0;
}
