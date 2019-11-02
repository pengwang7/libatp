#include <string>

namespace atp {

namespace socket {

/* Create a socket and set nonblocking */ 
int createNonblockingSocket();

/* Set socket is nonblocking */
void setNonblocking(int fd);

/* Set socket reuseable addr */
void setReuseAddr(int fd);

/* Set socket reuseable port */
void setReusePort(int fd);

/* Set listenning socket deferred ACK*/
void setTCPDeferred(int fd);

/* Set socket no delay for socket send data */
void setTCPNoDelay(int fd, bool on);

/* Set socket quick ack for socket recv data */
void setQuickAck(int fd, bool on);

/* Set socket keepalive */
void setKeepalive(int fd, bool on);
                                         
}/*end namespace socket*/

}/*end namespace atp*/
