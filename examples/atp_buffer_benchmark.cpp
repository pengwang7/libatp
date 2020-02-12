#include "glog/logging.h"

#include "atp_buffer.h"

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

    LOG(INFO) <<"ByteBuffer ByteBufferReader ByteBufferWriter tt.";

    ByteBuffer* byte_buffer = new ByteBuffer(8, 10);
    if (!byte_buffer) {
        LOG(ERROR) <<"New bytebuffer failed.";
        return -1;
    }

    ByteBufferWriter writer(*byte_buffer);
    ByteBufferReader reader(*byte_buffer);

	LOG(INFO) << "Buffer caps: " << byte_buffer->getCaps();
	
	const char* data = "12345678";
	const int size = strlen(data);
	const int count = 1000;
	
	for (int i = 0; i < count; ++ i) {
		writer.append(data, size);
	}
	
	writer.prependInt64(count * size);

	LOG(INFO) << "buffer len: " << reader.peekInt64();
	LOG(INFO) << "buffer caps: " << byte_buffer->getCaps();
	LOG(INFO) << "buffer data: " << reader.consume(reader.readInt64()).data();
	
	atp_logger_close();
    
    return 0;
}
