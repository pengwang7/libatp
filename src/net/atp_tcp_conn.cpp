#include <unistd.h>
#include <sys/types.h>

#include "net/atp_config.h"
#include "net/atp_channel.h"
#include "net/atp_tcp_conn.h"
#include "net/atp_libevent.h"
#include "net/atp_event_loop.h"

namespace atp {

Connection::Connection(EventLoop* event_loop, int fd, std::string id, std::string& remote_addr)
    : event_loop_(event_loop), fd_(fd), id_(id), remote_addr_(remote_addr) {

    /* Check the args is validity. */
    assert(event_loop_ != nullptr);
    assert(fd_ >= 0);
    assert(id_.length() > 0);
    assert(remote_addr_ != "");

    /*
     * Create channel for file description read and write,
     * and set channel read callback and write callback.
     */
    chan_.reset(new Channel(event_loop_, fd_, false, false));
    chan_->setReadCallback(std::bind(&Connection::netFdReadHandle, this));
    chan_->setWriteCallback(std::bind(&Connection::netFdWriteHandle, this));

    if (ATP_NET_DEBUG_ON) {
        LOG(INFO) << "[Connection] create connection: " << id_;
    }
}

Connection::~Connection() {
    ::close(fd_);
    fd_ = -1;

    if (ATP_NET_DEBUG_ON) {
        LOG(INFO) << "[~Connection] destroy connection: " << id_;
    }
}

void Connection::attachToEventLoop() {
    assert(event_loop_->threadSafety());

    /*
     * Every connection attach to event loop default enable read event.
     */
    chan_->enableEvents(true, false);

    if (conn_fn_) {
        conn_fn_(shared_from_this());
    }
}

void Connection::send(const void* data, size_t len) {
    if (!data || len <= 0) {
        return;
    }

    auto fn = [&]() {
        ssize_t nwrite = 0;
        size_t remaining_data_size = len;

        /*
         * If the write buffer unread bytes is not 0, it means had retransmissions data at last time.
         * Need to make sure that send all the retransmissions data first, and then send this(data) data
         * to make sure that write in order.
         * The channel's writable is true only if retransmission data needs to be written.
         */
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

        if (remaining_data_size > 0) {
            ByteBufferedWriter writer(write_buffer_);
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

    ByteBufferedReader reader(*buffer);
    size_t len = buffer->unreadBytes();

    send(reader.consume(len).void_type_data(), len);
}

void Connection::close() {
    auto self = shared_from_this();
    auto fn = [self]() {
        assert(self->event_loop_->threadSafety());
        self->netFdCloseHandle();
    };

    event_loop_->sendToQueue(fn);
}

void Connection::netFdReadHandle() {
    ByteBufferedReader reader(read_buffer_);
    if (reader.read(fd_) <= 0) {
        netFdErrorHandle();
        return;
    }

    if (read_fn_) {
        read_fn_(shared_from_this(), read_buffer_);
    }
}

void Connection::netFdWriteHandle() {
    /*
     * The write callback only for send retransmissions data,
     * when the write data size > file description kernel buffer size.
     */
    assert(chan_->isWritable());

    ssize_t n = ::send(fd_, write_buffer_.data(), write_buffer_.unreadBytes(), MSG_NOSIGNAL);
    if (n > 0) {
        ByteBufferedReader reader(write_buffer_);
        reader.remove(n);

        if (write_buffer_.unreadBytes() == 0) {
            chan_->disableEvents(false, true);
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

} /* end namespace atp */
