#ifndef __ATP_TIMING_CYCLE_H__
#define __ATP_TIMING_CYCLE_H__

#include <time.h>
#include <list>
#include <array>
#include <memory>
#include <string>
#include <functional>

#include "atp_debug.h"

namespace atp {

class TimingCycle {
public:
    using TimingEventTaskPtr = std::function<void()>;
    using TimingEventTaskList = std::list<TimingEventTaskPtr>;
    using TimingEventTaskListPtr = std::unique_ptr<TimingEventTaskList*>;
    using CycleContainer = std::array<TimingEventTaskListPtr, CONN_READ_WRITE_EXPIRES>;

public:
    explicit TimingCycle(size_t slot_size, time_t timing_step, std::string name) {
        name_ = name;
        slot_size_ = slot_size;
        timing_step_ = timing_step;

        if (!name_.size()) {
            name_ = "default timing cycle";
        }

        initSlot();
    }

    ~TimingCycle() {
    
    }

public:
    void start() {
    
    }

    void stop() {
    
    }

    bool addTask() {
        return true; 
    }

    bool removeTask() {
        return true; 
    }

private:
    void initSlot() {
    
    }

    time_t getSlotPosition() {
        time_t pos = -1;

        return pos;
    }

private:
    typedef struct {
        time_t expires_;
        TimingEventTaskListPtr timing_task_list_;
    } slot_node_t;

    std::string name_;
    size_t slot_size_;
    time_t timing_step_;
    CycleContainer cycle_polling_;
};

}/*end namespace atp*/

#endif
