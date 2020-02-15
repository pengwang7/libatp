#include <string>

namespace atp {

class SocketOptions {
public:
    virtual bool getOption(int opt_id) = 0;
    virtual void setOption(int opt_id, int on) = 0;
};

class SocketImpl : public SocketOptions {
protected:
    bool getOption(int opt_id) override;

    void setOption(int opt_id, int on) override;

protected:
    int create(bool stream);
    
    int connect(std::string& ip, int port);

    int bind(std::string& ip, int port);

    int listen(int backlog);

    int accept(std::string& remote_addr);

    void close();
    
protected:
    int fd_;
    int port_;
    std::string ip_;
};

/* Create a socket and set nonblocking */ 
int createNonblockingSocket();

}/*end namespace atp*/
