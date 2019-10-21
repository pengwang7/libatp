#include <stdlib.h>
#include <assert.h>

#include "atp_memory_pool.h"

/* Memory pool factory policy /new/delete/malloc/free */

namespace atp {

static void* chunk_alloc__(pool_factory_t* factory, size_t size);
static void chunk_free__(pool_factory_t* factory, void* mem, size_t size);
static void* chunk_new__(pool_factory_t* factory, size_t size);
static void chunk_delete__(pool_factory_t* factory, void* mem, size_t size);


pool_factory_policy_t default_policy {
    chunk_alloc__,
    chunk_free__,
};

pool_factory_policy_t new_policy {
    chunk_new__,
    chunk_delete__,
};

pool_factory_policy_t* get_factory_default_policy() {
    return &default_policy;
}

pool_factory_policy_t* get_factory_new_policy() {
    return &new_policy;
}

static void* chunk_alloc__(pool_factory_t* factory, size_t size) {
    assert(factory != NULL);

    if (factory->on_chunk_alloc) {
        factory->on_chunk_alloc(factory, size);
    }

    void* mem = calloc(1, size);
    if (!mem) {
        /* Allocate failed*/
	    return NULL;
    }

	return mem;
};

static void chunk_free__(pool_factory_t* factory, void* mem, size_t size) {
    assert(factory != NULL);

    if (factory->on_chunk_free) {
        factory->on_chunk_free(factory, size);
    }

    free(mem);
    mem = NULL;
}

static void* chunk_new__(pool_factory_t* factory, size_t size) {
    assert(factory != NULL);

    if (factory->on_chunk_alloc) {
        factory->on_chunk_alloc(factory, size);
    }

    void* mem = static_cast<void*>(new unsigned char[size]);
    if (!mem) {
        /* Allocate failed*/
	    return NULL;
    }

    return mem;
}

static void chunk_delete__(pool_factory_t* factory, void* mem, size_t size) {
    assert(factory != NULL);

    if (factory->on_chunk_free) {
        factory->on_chunk_free(factory, size);
    }

    delete[] (static_cast<unsigned char*>(mem));
    mem = NULL;
}

}/*end namespace atp*/
