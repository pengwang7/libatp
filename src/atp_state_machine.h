#ifndef __ATP_STATE_MACHINE_H__
#define __ATP_STATE_MACHINE_H__

#include <atomic>

namespace atp {

struct STATE_MACHINE_INTERFACE {
public:
    enum SERVICE_STATE {
        STATE_NULL          = 1 << 0,
        STATE_INIT          = 1 << 1,
        STATE_RUNNING       = 1 << 2,
        STATE_STOPPING      = 1 << 3,
        STATE_STOPPED       = 1 << 4,
    };

public:
    bool CHECK_STATE(SERVICE_STATE state) const {
        return state_.load() == state;
    }

    SERVICE_STATE STATE() {
        return state_.load();
    }
    
    std::string STATE_TO_STRING(SERVICE_STATE state) const {
        switch (state) {
            case STATE_NULL:
                return std::string("STATE-NULL");

            case STATE_INIT:
                return std::string("STATE-INIT");

            case STATE_RUNNING:
                return std::string("STATE-RUNNING");

            case STATE_STOPPING:
                return std::string("STATE-STOPPING");

            case STATE_STOPPED:
                return std::string("STATE-STOPPED");

            default:
                return std::string("STATE-INVALID");
        }
    }

protected:
    std::atomic<SERVICE_STATE> state_ = { STATE_NULL };
};

} /* end namespace atp */

#endif /* __ATP_STATE_MACHINE_H__ */
