#include "glog/logging.h"
#include "atp_memory_pool.h"

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

const int kMaxCapacity = 1024 * 1024 * 8;

void print_pool_info(pool_t* pl) {
    LOG(INFO) << "-Pool name: " << get_pool_name(pl);
    LOG(INFO) << "-Pool capacity: " << get_pool_capacity(pl);
}

void print_pool_helper_info(pool_helper_t* helper) {
    LOG(INFO) << "Pool helper reference: " << get_pool_helper_reference(helper);
    LOG(INFO) << "Pool helper max cap: " << get_pool_helper_max_capacity(helper);
    LOG(INFO) << "Pool helper used size: " << get_pool_helper_used_mem_size(helper);
    LOG(INFO) << "Pool helper cap: " << get_pool_helper_capacity(helper);
}

int main() {
    atp_logger_init();
    pool_helper_t* helper = create_pool_helper();
    if (helper == NULL) {
        LOG(ERROR) << "pool helper is nil.";
        return -1;
    }

    pool_helper_init(helper, NULL, kMaxCapacity);

    print_pool_helper_info(helper);

    LOG(INFO) << "--------------------------------------";

    pool_t* pl = create_pool(get_pool_helper_factory(helper), "test-001", 1024 * 1024 * 9, 2048);
    if (pl == NULL) {
        LOG(ERROR) << "create pool failed.";
        return -1;
    }

    print_pool_info(pl);
    LOG(INFO) << "######################################";

    print_pool_helper_info(helper);
    LOG(INFO) << "11111111111111111111111111111111111111111";
    int count = 2;

    while (count --) {
        char* buf = static_cast<char*>(pool_alloc(pl, 512));
        if (!buf) {
            LOG(ERROR) << "pool alloc failed.";
            return -1;
        }
    }

    release_pool(pl, get_pool_helper_factory(helper));

    print_pool_helper_info(helper);

    LOG(INFO) << "======================================";

    pool_helper_destroy(helper);

    atp_logger_close();

    return 0;
}
