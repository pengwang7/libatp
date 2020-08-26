#include "net/atp_timing_wheel.hpp"
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

void test_ring_buffer() {

}

void test_timing_wheel() {
#if 0
    std::shared_ptr<TimingWheel> t(new TimingWheel(10, 1));
    int uuid = 0;
    while (true) {
        uuid ++;
        LOG(INFO) << "TimingWheel push";
        ConnectionTestPtr conn(new test_conn(uuid));

        std::pair<int, ConnectionTestPtr> pair_value;
        pair_value = std::make_pair(uuid, conn);
        test_vec.insert(pair_value);

        EntryPtr entry(new Entry(conn));
        t->push_bucket(Bucket());
        t->push_back(entry);
        sleep(1);
    }
#endif
}

int main() {
    atp_logger_init();

    LOG(INFO) << "=========================\n";

    test_ring_buffer();

    test_timing_wheel();

    atp_logger_close();

    return 0;
}
