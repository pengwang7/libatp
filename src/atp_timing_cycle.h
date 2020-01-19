#ifndef __ATP_TIMING_CYCLE_H__
#define __ATP_TIMING_CYCLE_H__

#include <list>
#include <string>
#include <functional>
#include <unordered_set>

#include "atp_cbs.h"
#include "atp_debug.h"
#include "atp_tcp_conn.h"

//test
#include "glog/logging.h"

namespace atp {
/*

*/

template <class T>
class CircularBuffer {
public:
    explicit CircularBuffer() {
        size_ = 0;
    }

    explicit CircularBuffer(size_t size) {
        size_ = size;
    }

    ~CircularBuffer() {
        list_.clear();
    }

public:
    void push_back(T elem) {
        if (list_.size() == size_) {
            list_.pop_front();
        }
        
        list_.push_back(elem);
    }

    void traverse() {
        for (auto iter = list_.begin(); iter != list_.end(); ++ iter) {
            LOG(INFO) << "list: " << *iter;
        }
    }

    T& back() { return list_.back(); }

    void resize(size_t size) { size_ = size; }

    bool empty() { return list_.empty(); }
    
    size_t size() { return list_.size(); }

    bool full() { return list_.size() == size_; }
    
private:
    size_t size_;
    std::list<T> list_;
};

struct Entry;
using WeakEntryPtr = std::weak_ptr<Entry>;
using SharedEntryPtr = std::shared_ptr<Entry>;
using Bucket = std::unordered_set<SharedEntryPtr>;
using WeakConnectionPtr = std::weak_ptr<Connection>;
using WeakConnectionList = CircularBuffer<Bucket>;

struct Entry {
    public:
        Entry(const WeakConnectionPtr conn)
            : conn_(conn) {

        }

        ~Entry() {
            SharedConnectionPtr conn = conn_.lock();
            if (conn) {
                conn->close();
            }
        }

    public:
        WeakConnectionPtr conn_;
};

class TimingWheel {    
public:
    explicit TimingWheel(size_t slot_size, size_t timing_step, std::string name) {
        name_ = name;
        slot_size_ = slot_size;
        timing_step_ = timing_step;

        if (!name_.size()) {
            name_ = "Default Timing Wheel";
        }

        cycle_buckets_.resize(slot_size_);
    }

public:
    std::string display() { return name_; }
    
    void push_back(SharedEntryPtr elem) {
        cycle_buckets_.back().insert(elem);
    }

public:
//private:
    std::string name_;
    size_t slot_size_;
    size_t timing_step_;
    WeakConnectionList cycle_buckets_;
};

}/*end namespace atp*/

#endif
