#include <fcntl.h>
#include <unistd.h>
#include <sys/eventfd.h>

#include "atp_libevent.h"

namespace atp {

int                                                                              
evutil_make_internal_pipe(evutil_socket_t fd[2]) {                               
	if (pipe2(fd, O_NONBLOCK|O_CLOEXEC) == 0) {
		return 0;
	}
	
	return -1;
}

int
evutil_make_internal_eventfd(int val, int flags) {
    flags = (flags == 0) ? (EFD_NONBLOCK | EFD_CLOEXEC) : flags;
    return eventfd(val, flags);
}

}/*end namespace atp*/
