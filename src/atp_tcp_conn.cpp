#include <unistd.h>

#include "atp_channel.h"
#include "atp_tcp_conn.h"
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

void Connection::shutdown() {
    
}

int64_t Connection::send(const void* data, size_t len) {
	return 0;
}

int64_t Connection::send(Buffer* buffer) {
	return 0;
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

}

void Connection::netFdWriteHandle() {

}

void Connection::netFdCloseHandle() {

}

void Connection::netFdErrorHandle() {

}

} /*end namespace atp */
