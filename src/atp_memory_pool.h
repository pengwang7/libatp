#ifndef __ATP_MEMORY_POOL_H__
#define __ATP_MEMORY_POOL_H__

#include <stdio.h>
#include <pthread.h>
#include <sys/queue.h>

#include "atp_debug.h"

namespace atp {

struct pool;
struct pool_mgr;
struct pool_chunk;
struct pool_factory;
struct pool_factory_policy;

typedef struct pool pool_t;
typedef struct pool_chunk pool_chunk_t;
typedef struct pool_factory pool_factory_t;
typedef struct pool_factory_policy pool_factory_policy_t;
typedef struct pool_helper pool_helper_t;


/* The memory pool public interface */
ATP_EXPORT_SYMBOL size_t get_pool_capacity(pool_t* pool);
ATP_EXPORT_SYMBOL const char* get_pool_name(pool_t* pool);
ATP_EXPORT_SYMBOL pool_t* create_pool(pool_factory_t* factory, const char* name, size_t init_size, size_t incr_size);
ATP_EXPORT_SYMBOL void release_pool(pool_t* pool, pool_factory_t* factory);
ATP_EXPORT_SYMBOL void* pool_alloc(pool_t* pool, size_t size);

/* Factory policy for new/delete/malloc/free */
struct pool_factory_policy {
    void* (*chunk_alloc)(pool_factory_t* factory, size_t size);
    void (*chunk_free)(pool_factory_t* factory, void* mem, size_t size);
};

extern pool_factory_policy_t default_policy;
extern pool_factory_policy_t new_policy;

ATP_EXPORT_SYMBOL pool_factory_policy_t* get_factory_default_policy();
ATP_EXPORT_SYMBOL pool_factory_policy_t* get_factory_new_policy();


struct pool_factory {
    pool_factory_policy_t policy_;
    pool_t* (*create_pool)(pool_factory_t* factory, const char* name, size_t init_size, size_t incr_size);
    void (*release_pool)(pool_factory_t* factory, pool_t* pool);
    void (*dump_status)(pool_factory_t* factory, bool detail);
    void (*on_chunk_free)(pool_factory_t* factory, size_t size);
    void (*on_chunk_alloc)(pool_factory_t* factory, size_t size);
};


/* Unified management of memory pool */
/* The pool helper public interface */
ATP_EXPORT_SYMBOL pool_helper_t* create_pool_helper();
ATP_EXPORT_SYMBOL void pool_helper_init(pool_helper_t* ph, const pool_factory_policy_t* policy, size_t max_capacity);
ATP_EXPORT_SYMBOL void pool_helper_destroy(pool_helper* ph);
ATP_EXPORT_SYMBOL size_t get_pool_helper_reference(pool_helper* ph);
ATP_EXPORT_SYMBOL size_t get_pool_helper_capacity(pool_helper* ph);
ATP_EXPORT_SYMBOL size_t get_pool_helper_max_capacity(pool_helper* ph);
ATP_EXPORT_SYMBOL size_t get_pool_helper_used_mem_size(pool_helper* ph);
ATP_EXPORT_SYMBOL pool_factory_t* get_pool_helper_factory(pool_helper* ph);

}

#endif
