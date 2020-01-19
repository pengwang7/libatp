#include "atp_buffer.h"
#include "atp_libevent.h"

namespace atp {

void Buffer::reader(int fd, std::string& error) {
    char extrbuffer[65536];
    struct iovec iov[2];
    const size_t writable = writableBytes();
    iov[0].iov_base = buffer_ + write_index_;
    iov[0].iov_len = writable;
    iov[1].iov_base = extrbuffer;
    iov[1].iov_len = sizeof(extrbuffer);

    const int iov_count = (writable < sizeof(extrbuffer)) ? 2 : 1;
    const ssize_t n = readv(fd, iov, iov_count);
    if (n < 0) {
        if (EVUTIL_ERR_RW_RETRIABLE(errno)) {
            error = std::string("retriable");
        } else {
            error = std::string(strerror(errno));
        }
    } else if (static_cast<size_t>(n) <= writable) {
        write_index_ += n;
    } else {
        write_index_ = caps_;
        append(extrbuffer, n - writable);
    }

    if (n == 0) {
        error = std::string("closed");
    } else if (n > 0) {
        error = std::string("success");
    }
}

void Buffer::writer(int fd, std::string& error) {

}

}/*end namespace atp*/