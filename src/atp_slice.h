#ifndef __ATP_SLICE_H__
#define __APT_SLICE_H__

#include <assert.h>
#include <string.h>
#include <string>

namespace atp {

/* Slice仅仅保存指针,需要外部自己维护对象的生命周期,可以很好的用来替换std::string */
class Slice {
public:
    Slice() : data_(""), size_(0) {}
    
    Slice(const char* s, size_t n) : data_(s), size_(n) {}
    
    Slice(const char* s) : data_(s), size_(strlen(s)) {}
    
    Slice(const std::string& s) : data_(s.data()), size_(s.size()) {}
    
    Slice(const Slice&) = default;
    
    Slice& operator=(const Slice&) = default;

public:
    const char* data() const { return data_; }
    
    size_t size() const { return size_; }
    
    bool empty() const { return size_ == 0; }
    
    void clear() { data_ = ""; size_ = 0; }

    std::string ToString() const { return std::string(data_, size_); }

    /* Slice operator symbol [], !=, == */
    char operator[](size_t pos) const { assert(pos < size_); return data_[pos]; }

private:
    const char* data_;
    size_t size_;
};

/* Global operator */
inline bool operator==(const Slice& x, const Slice& y) { return ((x.size() == y.size()) && (memcmp(x.data(), y.data(), x.size()) == 0)); }

inline bool operator!=(const Slice& x, const Slice& y) { return !(x == y); }

using slice = Slice;

}/*end namespace atp*/

#endif
