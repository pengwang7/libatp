#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netdb.h>
#include <string>
#include <iostream>

#include "rpc.pb.h"
#include "echo_server.pb.h"
#include "glog/logging.h"

using namespace atp;

static void atp_logger_init() {
    FLAGS_alsologtostderr = true;
    FLAGS_colorlogtostderr = true;
    FLAGS_logbufsecs = 0;
    FLAGS_max_log_size = 1800;

    ::google::InitGoogleLogging("test");
    ::google::SetLogDestination(google::GLOG_INFO,"log-");
}

static void atp_logger_close() {
    ::google::ShutdownGoogleLogging();
}

int main() {
    atp_logger_init();

    int fd, numbytes;
    char buf[4096] = {0};
    struct sockaddr_in server;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(7765);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    int ret = connect(fd, (struct sockaddr *)&server, sizeof(struct sockaddr));
    if (ret < 0) {
        LOG(ERROR) << "Connect met error: " << strerror(errno);

        return -1;
    }

    std::string test = "hello, world";
    std::string message;
    std::string req_message;
    EchoRequest request;
    RpcMessage core_message;
    core_message.set_type(REQUEST);
    core_message.set_id(10011);
    core_message.set_service("atp.EchoService");
    core_message.set_method("Echo");

    request.set_message("Hello,World!");
    request.SerializeToString(&req_message);

    core_message.set_request(req_message);    
    core_message.SerializeToString(&message);

    ret = send(fd, message.c_str(), message.size(), 0);
    if (ret == message.size()) {
        LOG(INFO) << "Send message success";
    } else {
        LOG(ERROR) << "Send message failed";
    }

    RpcMessage r1;

    char buff[512] = {0};
    ret = recv(fd, buff, 512, 0);
    if (ret <= 0) {
        LOG(ERROR) << "Recv message failed: " << strerror(errno);
    }

    r1.ParseFromArray(buff, ret);

    LOG(INFO) << "===111==" << r1.id();
    LOG(INFO) << "===222==" << r1.type();
    LOG(INFO) << "===333==" << r1.response();
    close(fd);

    atp_logger_close();

    return 0;
}
