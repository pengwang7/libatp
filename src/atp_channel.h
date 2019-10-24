#ifndef __ATP_CHANNEL_H__
#define __ATP_CHANNEL_H__

#include <string>
#include <functional>

#include "atp_debug.h"

struct event;
struct event_base;

namespace atp {

class Channel {
public:
	using EventCallbackPtr = std::function<void()>;
	
public:
	Channel(EventLoop* event_loop, int fd, bool readable, bool writable);
	~Channel();

public:
	void attachToEventLoop();
	void detachFromEventLoop();
	void updateEvents();
	void close();

public:
	void enableEvents(bool readable, bool writable);
	void disableEvents(bool readable, bool writable);
	void disableAllEvents();
	int getInternalFd() const;
	std::string eventsToString() const;

public:
	/* Inline functions */
	void setReadCallback(const EventCallbackPtr& cb) {
		read_cb_ = cb; 
	}
	
	void setWriteCallback(const EventCallbackPtr& cb) {
		write_cb_ = cb;
	}

	bool isAttached() const {
		return attached_;
	}

	bool isNone() const {
		return (events_ == ATP_NONE_EVENT);
	}

	bool isReadable() const {
		return (events_ & ATP_READ_EVENT) != 0;
	}

	bool isWritable() const {
		return (events_ & ATP_WRITE_EVENT) != 0;
	}

private:
	void eventHandle(int fd, short which);
	static void eventHandle(int fd, short which, void* args);

private:
	EventLoop* event_loop_;			/* IO event loop */
	struct event* event_;			/* The libevent event */
	int fd_;						/* The event file description */
	int events_;					/* The event is read or write or both */
	bool attached_;					/* The event is weather or not add to libevent event_base */

	EventCallbackPtr read_cb_;		/* Libevent event read callback function */
	EventCallbackPtr write_cb_;		/* Libevent event write callback function */
};

}/*end namespace atp*/
#endif
