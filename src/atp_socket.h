#ifndef __ATP_SOCKET_H__
#define __ATP_SOCKET_H__

#include <string>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/tcp.h>


namespace atp {

typedef enum {
    SOCKET_ERROR_BASE          =  (-300),
    SOCKET_CREATE_ERROR        =  (SOCKET_ERROR_BASE - 1),
    SOCKET_SETOPT_ERROR        =  (SOCKET_ERROR_BASE - 2),
    SOCKET_GETOPT_ERROR        =  (SOCKET_ERROR_BASE - 3),
    SOCKET_SET_NONBLOCK_ERROR  =  (SOCKET_ERROR_BASE - 4),
    SOCKET_CONNECT_ERROR       =  (SOCKET_ERROR_BASE - 5),
    SOCKET_BIND_ERROR          =  (SOCKET_ERROR_BASE - 6),
    SOCKET_LISTEN_ERROR        =  (SOCKET_ERROR_BASE - 7),
    SOCKET_ACCEPT_ERROR        =  (SOCKET_ERROR_BASE - 9),
    SOCKET_CONNECT_TIMEOUT     =  (SOCKET_ERROR_BASE - 10),
    SOCKET_GET_APP_ERROR       =  (SOCKET_ERROR_BASE - 11)
} SocketError;

class SocketOptions {
public:
    virtual bool getOption(int fd, int opt_id) = 0;
    virtual void setOption(int fd, int opt_id, int on) = 0;
};

class SocketImpl : public SocketOptions {
public:
    explicit SocketImpl() {}
    virtual ~SocketImpl() {}
    
protected:
    bool getOption(int fd, int opt_id) override;

    void setOption(int fd, int opt_id, int on) override;

protected:
    void setfd(int fd);
    
    int create(bool stream);
    
    int connect(std::string& ip, int port, struct timeval* tv);

    int bind(std::string& ip, int port);

    int listen(int backlog);

    int accept(std::string& remote_addr);

    void close();
    
private:
    int fd_;
    int port_;
    std::string ip_;
};

}/*end namespace atp*/

#endif /* __ATP_SOCKET_H__ */
