#include "listener.h"

namespace atp {

Listener::Listener(EventLoop* event_loop, const std::string& address)
    : event_loop_(event_loop), fd_(-1) {

}

Listener::~Listener() {
    close(fd_);
    fd_ = -1;
}

void Listener::listenning() {

}

void Listener::accept() {

}

void Listener::stop() {

}

void Listener::accpetHandle() {

}

}/*end namespace atp*/
