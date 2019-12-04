#ifndef __ATP_DEBUG_H__
#define __ATP_DEBUG_H__

#include <assert.h>

namespace atp {

#define ATP_EXPORT_SYMBOL

#define ATP_DEBUG_ON (1)

#define ATP_MEMORY_POOL_NAME  (64)
#define ATP_MEMORY_POOL_ALIGN (8)
#define ATP_MEMORY_POOL_START (5)
#define ATP_MEMORY_POOL_CACHING_SIZE (16)

/* The atp event flags */
#define ATP_NONE_EVENT  (0x00)
#define ATP_READ_EVENT  (0x02)
#define ATP_WRITE_EVENT (0x04)

/* Listen queue max size */
#define ATP_SO_MAX_CONN (4096)

/* Buffer init size and reserved prepend size */
#define INIT_BUFFER_SIZE (1024)
#define RESERVED_PREPEND_SIZE (8)

/* The dynamic thread pool MAX threads */
#define THREAD_POOL_MAX_THREADS (128)

#define htonll(x)                             \
((((x) & 0xff00000000000000ull) >> 56)       \
| (((x) & 0x00ff000000000000ull) >> 40)      \
| (((x) & 0x0000ff0000000000ull) >> 24)      \
| (((x) & 0x000000ff00000000ull) >> 8)       \
| (((x) & 0x00000000ff000000ull) << 8)       \
| (((x) & 0x0000000000ff0000ull) << 24)      \
| (((x) & 0x000000000000ff00ull) << 40)      \
| (((x) & 0x00000000000000ffull) << 56))

#define ntohll(x) htonll(x)

/* Memory alignment */
// example: 127以8个字节进行内存对齐,此处假设字长为8
// 127 + (-127) & (8 - 1)
// 127补码: 01111111, -127补码: 10000001
// -127 & (8 - 1) = 1
// res: 128
#define ATP_ALIGN_PTR(PTR, ALIGNMENT) (PTR + (-(ssize_t)(PTR) & (ALIGNMENT - 1)))    

#define event_safe_free(_var, _freefn) do { _freefn((_var)), (_var) = NULL; } while(0)

}/*end namespace atp*/
#endif
