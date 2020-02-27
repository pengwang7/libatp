#ifndef __ATP_STATUS_H__
#define __ATP_STATUS_H__

#include <atomic>

namespace atp {

struct STATE_MECHINE_INTERFACE {
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

protected:
    std::atomic<SERVICE_STATE> state_ = { STATE_NULL };
};

} /* end namespace atp */

#endif /* ATP_STATUS_H__ */
