#ifndef __ATP_HTTPS_CLIENT_H__
#define __ATP_HTTPS_CLIENT_H__

namespace atp {

class HttpsClient {
public:
    HttpsClient();

    ~HttpsClient();

public:
    int doPost(std::string data, int data_type);
};

} /* end namespace atp */

#endif /* __ATP_HTTPS_CLIENT_H__ */