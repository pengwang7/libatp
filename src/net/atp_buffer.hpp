/*
 * MIT License
 *
 * Copyright (c) 2019 pengwang7(https://github.com/pengwang7/libatp)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __ATP_BUFFER_H__
#define __ATP_BUFFER_H__

#include "net/atp_config.h"
#include "net/atp_libevent.h"
#include "net/atp_io.hpp"
#include "app/atp_slice.hpp"

namespace atp {

class ByteBuffer {
public:
    ByteBuffer(size_t prepend_size = RESERVED_PREPEND_SIZE, size_t initial_size = INIT_BUFFER_SIZE) {
        // Init the ByteBuffer core variable, the default buffer size [8 header|1024 body].
        caps_ = prepend_size + initial_size;
        reserved_prepend_size_ = prepend_size;
        read_index_ = write_index_ = prepend_size;

        buff_ = new(std::nothrow) char[caps_];
        memset(buff_, 0, caps_);

        assert(buff_ != NULL);
    }

    ~ByteBuffer() {
        delete[] buff_;
        buff_ = NULL;

        caps_ = 0;
        read_index_ = 0;
        write_index_ = 0;
        reserved_prepend_size_ = 0;
    }

public:
    char* data() {
        return buff_ + read_index_;
    }

    const char* data() const {
        return buff_ + read_index_;
    }

    char* swap(char* buff) {
        char* tmp = buff_;
        buff_ = buff;

        return tmp;
    }

    char* buff() const {
        return buff_;
    }

    // Update the core buffer read/write index, if don't need any index, set 0.
    void updateReadWriteIndex(size_t read_index, size_t write_index, bool increment) {
        if (increment) {
            if (read_index != 0) {
                read_index_ += read_index;
            }

            if (write_index != 0) {
                write_index_ += write_index;
            }
        } else {
            if (read_index != 0) {
                read_index_ = read_index;
            }

            if (write_index != 0) {
                write_index_ = write_index;
            }
        }
    }

    size_t prependableBytes() const {
        return read_index_;
    }

    size_t prependHeaderBytes() const {
        return reserved_prepend_size_;
    }

    size_t writableBytes() const {
        assert(caps_ >= write_index_);
        return caps_ - write_index_;
    }

    size_t unreadBytes() const {
        assert(write_index_ >= read_index_);
        return write_index_ - read_index_;
    }

    size_t length() const {
        return unreadBytes();
    }

    void setNewCaps(size_t new_caps) {
        caps_ = new_caps;
    }

    size_t getCaps() const {
        return caps_;
    }

    char* writeBegin() {
        return buff_ + write_index_;
    }

    const char* writeBegin() const {
        return buff_ + write_index_;
    }

    void reset() {
        read_index_ = write_index_ = reserved_prepend_size_;
    }

private:
    char*  buff_;
    size_t caps_;
    size_t read_index_;
    size_t write_index_;
    size_t reserved_prepend_size_;
};


class ByteBufferedWriter : public IWriter {
public:
    explicit ByteBufferedWriter(ByteBuffer& buffer) : buffer_(buffer) {}

    ~ByteBufferedWriter() {}

public:
    void grow(size_t length) {
        size_t unread_bytes = buffer_.unreadBytes();
        size_t reserved_prepend_size = buffer_.prependHeaderBytes();

        if (buffer_.prependableBytes() + buffer_.writableBytes() < reserved_prepend_size + length) {
            size_t new_size = (buffer_.getCaps() << 1) + length;
            char* new_mem = new(std::nothrow) char[new_size];
            assert(new_mem);

            memset(new_mem, 0, new_size);
            memcpy(new_mem + reserved_prepend_size, buffer_.data(), unread_bytes);

            delete[] (buffer_.swap(new_mem));

            size_t new_write_index = reserved_prepend_size + unread_bytes;
            buffer_.updateReadWriteIndex(reserved_prepend_size, new_write_index, false);
            buffer_.setNewCaps(new_size);
        } else {
            char* buff = buffer_.buff();
            assert(reserved_prepend_size < buffer_.prependableBytes());
            memmove(buff + reserved_prepend_size, buffer_.data(), unread_bytes);
            size_t new_write_index = reserved_prepend_size + unread_bytes;
            buffer_.updateReadWriteIndex(reserved_prepend_size, new_write_index, false);

            assert(buffer_.unreadBytes() == unread_bytes);
            assert(buffer_.writableBytes() >= length);
        }
    }

    void prependInt32(int32_t x) {
        assert(sizeof(x) <= buffer_.prependableBytes());
        int32_t net_x = htonl(x);
        buffer_.updateReadWriteIndex(- sizeof(net_x), 0, true);
        memcpy(buffer_.data(), &net_x, sizeof(net_x));
    }

    void prependInt64(int64_t x) {
        assert(sizeof(x) <= buffer_.prependableBytes());
        int64_t net_x = htonll(x);
        buffer_.updateReadWriteIndex(- sizeof(net_x), 0, true);
        memcpy(buffer_.data(), &net_x, sizeof(net_x));
    }

    void append(const char* data, const size_t length) {
        if (buffer_.writableBytes() < length) {
            grow(length);
        }

        memmove(buffer_.writeBegin(), data, length);
        buffer_.updateReadWriteIndex(0, length, true);
    }

    void append(const void* data, const size_t length) {
        append(static_cast<const char*>(data), length);
    }

    void append(const slice& ss) {
        append(ss.data(), ss.size());
    }

    void appendInt32(int32_t x) {
        int32_t net_x = htonl(x);
        append(&net_x, sizeof(net_x));
    }

    void appendInt64(int64_t x) {
        int64_t net_x = htonll(x);
        append(&net_x, sizeof(net_x));
    }

    ssize_t write(int fd, const char* data, size_t size) override {
        ssize_t n = ::send(fd, data, size, MSG_NOSIGNAL);
        if (n < 0 && EVUTIL_ERR_RW_RETRIABLE(errno)) {
            return RETRIABLE_ERROR;
        }

        return n;
    }

    ssize_t writev(int fd, const struct iovec* iov, int iov_size) override {
        // TODO: Implement the vector write.
        return -1;
    }

private:
    ByteBuffer& buffer_;
};


class ByteBufferedReader : public IReader {
public:
    explicit ByteBufferedReader(ByteBuffer& buffer) : buffer_(buffer) {}

    ~ByteBufferedReader() {}

public:
    void decrease(size_t length) {
        if (length == 0) {
            return;
        }

        size_t available_len = buffer_.getCaps() - buffer_.prependHeaderBytes() - buffer_.unreadBytes();
        if (buffer_.writableBytes() < length && available_len >= length) {
            size_t unread_bytes = buffer_.unreadBytes();
            memmove(buffer_.buff() + buffer_.prependHeaderBytes(), buffer_.data(), unread_bytes);
            assert(unread_bytes = buffer_.unreadBytes());
        }

        if (buffer_.writableBytes() >= length) {
            size_t reserved_prepend_size = buffer_.prependHeaderBytes();
            size_t new_size = buffer_.getCaps() + reserved_prepend_size - length;
            char* new_mem = new(std::nothrow) char[new_size];

            assert(new_mem);

            memset(new_mem, 0, new_size);
            memcpy(new_mem + reserved_prepend_size, buffer_.data(), buffer_.unreadBytes());

            buffer_.setNewCaps(new_size);
            delete[] (buffer_.swap(new_mem));

            buffer_.updateReadWriteIndex(reserved_prepend_size, reserved_prepend_size + buffer_.unreadBytes(), false);
        }
    }

    void remove(size_t length) {
        if (length < buffer_.unreadBytes()) {
            buffer_.updateReadWriteIndex(length, 0, true);
        } else {
            buffer_.reset();
        }
    }

    int32_t peekInt32() {
        if (buffer_.unreadBytes() < sizeof(int32_t)) {
            return -1;
        }

        int32_t x = 0;
        memcpy(&x, buffer_.data(), sizeof(x));

        return ntohl(x);
    }

    int64_t peekInt64() {
        if (buffer_.unreadBytes() < sizeof(int64_t)) {
            return -1;
        }

        int64_t x = 0;
        memcpy(&x, buffer_.data(), sizeof(x));

        return ntohll(x);
    }

    int32_t readInt32() {
        int32_t x = peekInt32();
        if (x != -1) {
            remove(sizeof(x));
        }

        return x;
    }

    int64_t readInt64() {
        int64_t x = peekInt64();
        if (x != -1) {
            remove(sizeof(x));
        }

        return x;
    }

    ssize_t read(int fd, char* buf, size_t size) override {
        // TODO: Implement the stream read.
    }

    ssize_t readv(int fd, const struct iovec* iovs, size_t iovs_size) override {
        /*
         * In ByteBufferedReader, read the data to bytebuffer, not to IO vector.
         * Check the iov、iov_size must NULL and 0. 
         */
        assert(!iovs && !iovs_size);

        char extrbuffer[65536] = {0};
        struct iovec iov[2];
        const size_t writable = buffer_.writableBytes();
        iov[0].iov_base = buffer_.writeBegin();
        iov[0].iov_len = writable;
        iov[1].iov_base = extrbuffer;
        iov[1].iov_len = sizeof(extrbuffer);
        
        const int iov_count = (writable < sizeof(extrbuffer)) ? 2 : 1;
        const ssize_t n = ::readv(fd, iov, iov_count);
        if (n <= 0) {
            if (EVUTIL_ERR_RW_RETRIABLE(errno)) {
                return RETRIABLE_ERROR;
            } else {
                return n;
            }
        } else if (static_cast<size_t>(n) <= writable) {
            buffer_.updateReadWriteIndex(0, n, true);
        } else {
            buffer_.updateReadWriteIndex(0, buffer_.getCaps(), false);
            ByteBufferedWriter writer(buffer_);
            writer.grow(n - writable);
            writer.append(extrbuffer);
        }
        
        return n;

    }

    slice consume(const size_t length, bool verifiy) {
        if (length <= buffer_.unreadBytes()) {
            slice ss(buffer_.data(), length);
            remove(length);

            return ss;
        }

        if (verifiy) {
            slice ss2;

            return ss2;
        }

        slice ss3(buffer_.data(), buffer_.unreadBytes());
        remove(buffer_.unreadBytes());

        return ss3;
    }

    slice consume(const size_t length) {
        if (length <= buffer_.unreadBytes()) {
            slice ss(buffer_.data(), length);
            remove(length);

            return ss;
        }

        slice ss(buffer_.data(), buffer_.unreadBytes());
        remove(buffer_.unreadBytes());

        return ss;
    }

private:
    ByteBuffer& buffer_;
};

} /* end namespace atp */

#endif /* __ATP_BUFFER_H__ */
