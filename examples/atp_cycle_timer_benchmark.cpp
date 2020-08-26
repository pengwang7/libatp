#include "net/atp_event_loop.h"
#include "glog/logging.h"

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

int main() {
    atp_logger_init();
    LOG(INFO) << "test";
    //std::shared_ptr<CycleTimer> addCycleTask(int delay_ms, const TaskEventPtr& task, bool persist);

    {
        std::unique_ptr<EventLoop> event_loop(new EventLoop());
        event_loop->addCycleTask(5, []() {
            LOG(INFO) << "timedout";
        }, false);

        event_loop->dispatch();
    }

    atp_logger_close();

    return 0;
}
