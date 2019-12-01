#ifndef __ATP_BUFFER_H__
#define __ATP_BUFFER_H__

#include <arpa/inet.h>

#include "atp_debug.h"
#include "atp_slice.h"

namespace atp {

class Buffer {
public:
    Buffer(size_t prepend_size = RESERVED_PREPEND_SIZE, size_t initial_size = INIT_BUFFER_SIZE) {
    /* The prepend_size is the header size, initial_size is the body size */
        caps_ = prepend_size + initial_size;
        reserved_prepend_size_ = prepend_size;
        read_index_ = write_index_ = prepend_size;

        buffer_ = new(std::nothrow) char[caps_];
		
        assert(buffer_ != NULL);
    }
	
    ~Buffer() {
        delete[] buffer_;
        buffer_ = NULL;

        caps_ = 0;
        read_index_ = 0;
        write_index_ = 0;
        reserved_prepend_size_ = 0;
    }

public:
    /* The buffer general interface */
    size_t prependableBytes() const {
        return read_index_;
    }

    /* The buffer can writable bytes */
    size_t writableBytes() const {
        assert(caps_ >= write_index_);
        return caps_ - write_index_;
    }

    /* The buffer can readable bytes */
    size_t unreadBytes() const {
        assert(write_index_ >= read_index_);
        return write_index_ - read_index_;
    }

    size_t getCaps() const {
        return caps_;
    }

    char* writeBegin() {
        return buffer_ + write_index_;
    }
	
    const char* writeBegin() const {
        return buffer_ + write_index_;
    }

public:
    int32_t peekInt32() {
        if (unreadBytes() < sizeof(int32_t)) {
            return -1;
        }

        int32_t x = 0;
        memcpy(&x, data(), sizeof(x));

        /* The network byte order convert to local byte order */
        return ntohl(x);
    }

    int64_t peekInt64() {
        if (unreadBytes() < sizeof(int64_t)) {
            return -1;
        }

        int64_t x = 0;
        memcpy(&x, data(), sizeof(x));

        /* The network byte order convert to local byte order */
        return ntohll(x);
    }

    int32_t readInt32() {
        int32_t x = peekInt32();
        remove(sizeof(x));

        return x;
    }

    int64_t readInt64() {
        int64_t x = peekInt64();
        remove(sizeof(x));

        return x;
    }

    void appendInt32(int32_t x) {
        int32_t net_x = htonl(x);
        append(&net_x, sizeof(net_x));
    }

    void appendInt64(int64_t x) {
        int64_t net_x = htonll(x);
        append(&net_x, sizeof(net_x));
    }

    void prependInt32(int32_t x) {
        assert(sizeof(x) <= prependableBytes());
        int32_t net_x = htonl(x);
        read_index_ -= sizeof(net_x);
        memcpy(buffer_ + read_index_, &net_x, sizeof(net_x));
    }

    void prependInt64(int64_t x) {
        assert(sizeof(x) <= prependableBytes());
        int64_t net_x = htonll(x);
        read_index_ -= sizeof(net_x);
        memcpy(buffer_ + read_index_, &net_x, sizeof(net_x));
    }
    
public:
    /* The buffer read/write/reset interface */
    void reset() {
        read_index_ = reserved_prepend_size_;
        write_index_ = reserved_prepend_size_;
    }

    void remove(const size_t length) {
        if (length < unreadBytes()) {
            read_index_ += length;
        } else {
            reset();
        }
    }
   
    void append(const char* data, size_t const length) {
        if (writableBytes() < length) {
            grow(length);
        }

        memcpy(writeBegin(), data, length);
        write_index_ += length;
    }

    void append(const void* data, const size_t length) {
        append(static_cast<const char*>(data), length);
    }

    void append(const slice& ss) {
        append(ss.data(), ss.size());
    }
    
    slice retrieve(const size_t length) {
        if (length < unreadBytes()) {
            slice ss(data(), length);
            return ss;
        }

        slice ss(data(), unreadBytes());

        return ss;
    }

    /* The reader read from socket to the local buffer */
    void reader(int fd, std::string& error);
    void writer(int fd, std::string& error);
    
private:
    const char* data() const {
        return buffer_ + read_index_;
    }

    /* Judge the buffer whether or not have enough size, resize or move */
    void grow(size_t length) {
        size_t unread_bytes = unreadBytes();
		
        if (prependableBytes() + writableBytes() < reserved_prepend_size_ + length) {
            size_t new_size = (caps_ << 1) + unreadBytes();
            char* new_mem = new(std::nothrow) char[new_size];
            assert(new_mem);

            memcpy(new_mem + reserved_prepend_size_, buffer_ + read_index_, unread_bytes);
            caps_ = new_size;
            
            delete[] buffer_;
            buffer_ = new_mem;
        } else {
            assert(reserved_prepend_size_ < read_index_);
            memmove(buffer_ + reserved_prepend_size_, buffer_ + read_index_, unread_bytes);
        }

        read_index_ = reserved_prepend_size_;
        write_index_ = read_index_ + unread_bytes;
        
        assert(unreadBytes() == unread_bytes);
        assert(writableBytes() >= length);
    }
	
private:
    char* buffer_;					/* The buffer internal data pointer */
    size_t caps_;					/* The buffer capacity, it also buffer_ length */
    size_t read_index_;				/* The buffer read index */
    size_t write_index_;			/* The buffer write index */
    size_t reserved_prepend_size_;  /* The buffer reserved prepend size, default 8 bytes */
};

}/*end namespace atp*/
#endif
