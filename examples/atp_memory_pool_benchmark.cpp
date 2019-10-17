#include "glog/logging.h"
//#include "atp_memory_pool.h"

void atp_logger_init() {                                                      
    FLAGS_alsologtostderr = true;                                                
    FLAGS_colorlogtostderr = true;                                               
    FLAGS_logbufsecs = 0;                                                        
    FLAGS_max_log_size = 1800;                                                   
                                                                                 
    google::InitGoogleLogging("test");                                           
    google::SetLogDestination(google::GLOG_INFO,"log-");                         
}

int main() {
    atp_logger_init();

    LOG(DEBUG) << "hello, world";

    return 0;
}
