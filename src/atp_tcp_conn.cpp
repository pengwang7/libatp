#include <unistd.h>
#include <sys/types.h>

#include "atp_channel.h"
#include "atp_tcp_conn.h"
#include "atp_libevent.h"
#include "atp_event_loop.h"

namespace atp {

Connection::Connection(EventLoop* event_loop, int fd, int id, std::string& remote_addr)
	: event_loop_(event_loop), fd_(fd), id_(id), remote_addr_(remote_addr) {

	/* Check the args is validity. */
	assert(event_loop_ != nullptr);
	assert(fd_ > 0);
	assert(id_ >= 0);
	assert(remote_addr_ != "");

	/* Construct event channel and init channel read and write callback function. */
	chan_.reset(new Channel(event_loop_, fd_, false, false));
	chan_->setReadCallback(std::bind(&Connection::netFdReadHandle, this));
	chan_->setWriteCallback(std::bind(&Connection::netFdWriteHandle, this));
}

Connection::~Connection() {
	::close(fd_);
	fd_ = -1;
}

void Connection::attachToEventLoop() {
    auto self = shared_from_this();
    auto fn = [self]() {
        assert(self->event_loop_->safety());
        self->chan_->enableEvents(true, false);
        self->chan_->attachToEventLoop();
    };

    event_loop_->sendToQueue(fn);
}

void Connection::send(const void* data, size_t len) {
    if (!data || len <= 0) {
        return;
    }

    auto fn = [&]() {
        ssize_t nwrite = 0;
        size_t remaining_data_size = len;

        /* In gerernal, however, send data to peer dose not pass through channel and write buffer. */
        if (!chan_->isWritable() && write_buffer_.unreadBytes() == 0) {
            nwrite = ::send(fd_, static_cast<const char*>(data), len, MSG_NOSIGNAL);
            if (nwrite >= 0) {
                remaining_data_size -= nwrite;
                if (remaining_data_size == 0 && write_complete_fn_) {
                    write_complete_fn_(shared_from_this());
                }
            } else if (!EVUTIL_ERR_RW_RETRIABLE(errno)) {
                netFdErrorHandle();
            }
        }

        /* Special case only for send remaining data to peer. */
        if (remaining_data_size > 0) {
            ByteBufferWriter writer(write_buffer_);
            writer.append(static_cast<const char*>(data) + nwrite, remaining_data_size);
            chan_->enableEvents(false, true);
        }
    };

    event_loop_->sendToQueue(fn);
}

void Connection::send(ByteBuffer* buffer) {
    if (!buffer) {
        return;
    }

    ByteBufferReader reader(*buffer);
    size_t len = buffer->unreadBytes();

    send(reader.consume(len).void_type_data(), len);
}

void Connection::close() {
    auto self = shared_from_this();
    auto fn = [self]() {
        assert(self->event_loop_->safety());
        self->netFdCloseHandle();
    };

    event_loop_->sendToQueue(fn);
}

void Connection::netFdReadHandle() {
    ByteBufferReader reader(read_buffer_);
    if (reader.read(fd_) < 0) {
        netFdErrorHandle();
    } else {
        read_fn_(shared_from_this(), &read_buffer_);
    }
}

void Connection::netFdWriteHandle() {
    assert(chan_->isWritable());
    
    /* The handle only for send remaining data, when the write buffer data size > fd kernel buffer size. */
    ssize_t n = ::send(fd_, write_buffer_.data(), write_buffer_.unreadBytes(), MSG_NOSIGNAL);
    if (n > 0) {
        ByteBufferReader reader(write_buffer_);
        reader.remove(n);

        if (write_buffer_.unreadBytes() == 0) {
            chan_->disableEvents(true, false);
            if (write_complete_fn_) {
                write_complete_fn_(shared_from_this());
            }
        }
    } else if (!EVUTIL_ERR_RW_RETRIABLE(errno)) {
        netFdErrorHandle();
    }
}

void Connection::netFdCloseHandle() {
    chan_->disableAllEvents();
    chan_->close();

    if (close_fn_) {
        close_fn_(shared_from_this());
    }
}

void Connection::netFdErrorHandle() {
    netFdCloseHandle();
}

} /*end namespace atp */
