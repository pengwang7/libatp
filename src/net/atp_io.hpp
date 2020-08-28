#ifndef __ATP_IO_HPP__
#define __ATP_IO_HPP__

#include "sys/uio.h"

namespace atp {

class IStreamReader {
public:
    IStreamReader() {}

    virtual ~IStreamReader() {}

public:
    virtual ssize_t read(int fd, char* buf, size_t size) = 0;
};

class IVectorReader {
public:
    IVectorReader() {}

    virtual ~IVectorReader() {}

public:
    virtual ssize_t readv(int fd, const struct iovec* iov, size_t iov_size) = 0;
};

class IStreamWriter {
public:
    IStreamWriter() {}

    virtual ~IStreamWriter() {}

public:
    virtual ssize_t write(int fd, char* buf, size_t size) = 0;
};

class IVectorWriter {
public:
    IVectorWriter() {}

    virtual ~IVectorWriter() {}

public:
    virtual ssize_t writev(int fd, const struct iovec* iov, int iov_size) = 0;
};

class IReader : virtual public IStreamReader, virtual public IVectorReader {
public:
    IReader() {}

    virtual ~IReader() {}
};

class IWriter : virtual public IStreamWriter, virtual public IVectorWriter {
public:
    IWriter() {}

    virtual ~IWriter() {}
};

class IReaderWriter : virtual public IReader, virtual public IWriter {
public:
    IReaderWriter() {}

    virtual ~IReaderWriter() {}
};

} /* end namespace atp */

#endif /* __ATP_IO_HPP__ */
