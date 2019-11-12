#ifndef __ATP_BUFFER_H__
#define __ATP_BUFFER_H__

#include "atp_debug.h"

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
		return reserved_prepend_size_;
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

	const char* getData() const {
		return buffer_ + read_index_;
	}

	char* beginWrite() {
		return buffer_ + write_index_;
	}
	
	const char* beginWrite() const {
		return buffer_ + write_index_;
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
