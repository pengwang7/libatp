#ifndef __ATP_LIBEVENT_H__
#define __ATP_LIBEVENT_H__

#include <errno.h>

#include "libevent/event2/util.h"
#include "libevent/event2/event.h"
#include "libevent/event2/event_struct.h"

namespace atp {

#ifndef _WIN32

#if EAGAIN == EWOULDBLOCK
#define EVUTIL_ERR_IS_EAGAIN(e) \
	((e) == EAGAIN)
#else
#define EVUTIL_ERR_IS_EAGAIN(e) \
	((e) == EAGAIN || (e) == EWOULDBLOCK)
#endif

/* True iff e is an error that means a read/write operation can be retried. */
#define EVUTIL_ERR_RW_RETRIABLE(e)				\
	((e) == EINTR || EVUTIL_ERR_IS_EAGAIN(e))
/* True iff e is an error that means an connect can be retried. */
#define EVUTIL_ERR_CONNECT_RETRIABLE(e)			\
	((e) == EINTR || (e) == EINPROGRESS)
/* True iff e is an error that means a accept can be retried. */
#define EVUTIL_ERR_ACCEPT_RETRIABLE(e)			\
	((e) == EINTR || EVUTIL_ERR_IS_EAGAIN(e) || (e) == ECONNABORTED)

/* True iff e is an error that means the connection was refused */
#define EVUTIL_ERR_CONNECT_REFUSED(e)					\
	((e) == ECONNREFUSED)

#else
/* Win32 */

#define EVUTIL_ERR_IS_EAGAIN(e) \
	((e) == WSAEWOULDBLOCK || (e) == EAGAIN)

#define EVUTIL_ERR_RW_RETRIABLE(e)					\
	((e) == WSAEWOULDBLOCK ||					\
	    (e) == WSAEINTR)

#define EVUTIL_ERR_CONNECT_RETRIABLE(e)					\
	((e) == WSAEWOULDBLOCK ||					\
	    (e) == WSAEINTR ||						\
	    (e) == WSAEINPROGRESS ||					\
	    (e) == WSAEINVAL)

#define EVUTIL_ERR_ACCEPT_RETRIABLE(e)			\
	EVUTIL_ERR_RW_RETRIABLE(e)

#define EVUTIL_ERR_CONNECT_REFUSED(e)					\
	((e) == WSAECONNREFUSED)

#endif

/* Export the libevent internal function to this */
int
evutil_make_internal_pipe(evutil_socket_t fd[2]);

}/*end namespace atp*/
#endif
