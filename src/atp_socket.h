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

class SocketOptions {
public:
    virtual bool getOption(int opt_id) = 0;
    virtual void setOption(int opt_id, int on) = 0;
};

class SocketImpl : public SocketOptions {
public:
    SocketImpl() {}
    ~SocketImpl() {}
    
protected:
    bool getOption(int opt_id) override;

    void setOption(int opt_id, int on) override;

protected:
    void setfd(int fd);
    
    int create(bool stream);
    
    int connect(std::string& ip, int port);

    int bind(std::string& ip, int port);

    int listen(int backlog);

    int accept(std::string& remote_addr);

    void close();
    
private:
    int fd_;
    int port_;
    int flags_;
    std::string ip_;
};

}/*end namespace atp*/

#endif /* __ATP_SOCKET_H__ */
