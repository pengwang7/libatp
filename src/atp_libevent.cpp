#include <fcntl.h>
#include <unistd.h>

#include "libevent.h"

namespace atp {

int                                                                              
evutil_make_internal_pipe(evutil_socket_t fd[2]) {                               
	if (pipe2(fd, O_NONBLOCK|O_CLOEXEC) == 0) {
		return 0;
	}
	
	return -1;
}

}/*end namespace atp*/
