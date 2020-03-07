#ifndef __ATP_UUID_H__
#define __ATP_UUID_H__

#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <uuid/uuid.h>

#include <string>
#include <algorithm>

namespace atp {

/* NOTE: yum -y install libuuid-devel */
class UUIDGenerator {
public:
    UUIDGenerator() {
        uuid_t uuid;
        has_dev_urandom_ = 0;
        int dev_urandom_fd = open("/dev/urandom", O_RDONLY);
        assert(dev_urandom_fd >= 0);

        has_dev_urandom_ = 1;
        close(dev_urandom_fd);

        uuid_generate_random(uuid);
    }

    ~UUIDGenerator() {
        has_dev_urandom_ = 0;
    }

public:
    void generateUUID(uuid_t uuid) {
        uuid_generate_random(uuid);
    }

    std::string generateUUID() {
        uuid_t uuid;
        generateUUID(uuid);
        return toString(uuid);
    }

    uuid_t* generateUUID(std::string uuid) {
        return toUUID(uuid);
    }

    int compareUUID(uuid_t l, uuid_t r) {
        return uuid_compare(l, r);
    }

    void clear(uuid_t uuid) {
        uuid_clear(uuid);
    }
    
    int empty(uuid_t uuid) {
        return uuid_is_null(uuid);
    }

protected:
    std::string toString(uuid_t uuid) {
        char data[128];
        memset(data, 0, sizeof(data));
        uuid_unparse(uuid, data);
        std::string str(data);
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);

        return str;
    }

    uuid_t* toUUID(std::string uuid) {
        uuid_t* uu = (uuid_t*)calloc(1, sizeof(*uu));
        if (!uu) {
            return nullptr;
        }

        int res = uuid_parse(uuid.c_str(), *uu);
        if (res) {
            free(uu);
            return nullptr;
        }

        return uu;
    }
    
private:
    int has_dev_urandom_;
};

}/*end namespace atp*/

#endif /* __ATP_UUID_H__ */
