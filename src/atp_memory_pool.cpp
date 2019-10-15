#include "atp_debug.h"
#include "atp_memory_pool.h"

namespace atp {

/* The memory pool init size will be auto adjusted one of pool_size_array */
static size_t pool_size_array[ATP_MEMORY_POOL_CACHING_SIZE] = {

};

/* 内存池的内部以pool_chunk为单位进行内存分配 */
struct pool_chunk {
	unsigned char* buf_;
	unsigned char* cur_;
	unsigned char* end_;

	TAILQ_ENTRY(pool_chunk) entry;
};

struct pool {
	char   name_[ATP_MEMORY_POOL_NAME];
	size_t capacity_;
	size_t incr_size_;
	void*  data_;

	pool_factory_t* factory_;

	TAILQ_HEAD(, pool_chunk) chunk_list_;
	TAILQ_ENTRY(pool) entry;
};

struct pool_helper {
    pool_factory_t       factory_;                                               
    size_t               capacity_;                                              
    size_t               max_capacity_;                                          
    size_t               used_count_;                                            
    size_t               used_size_;                                             
    void*                data_;                                                  
    pthread_mutex_t      mutex_;                                                 
                                                                                 
    TAILQ_HEAD(, pool) used_pool_list_;
    TAILQ_HEAD(, pool) free_pool_list_[CACHING_POOL_ARRAY_SIZE];
};


static void* pool_alloc_find(pool_t* pool, size_t size);
static void* pool_alloc_from_chunk(pool_chunk_t* chunk, size_t size);
static pool_chunk_t* create_chunk_append_pool(pool_t* pool, size_t chunk_size);
static pool_t* create_pool__(pool_factory_t* factory, const char* name, size_t init_size, size_t incr_size);
static void release_pool__(pool_factory_t* factory, pool_t* pool);
static void release_pool2(pool_t* pool);
static void dump_status__(pool_factory_t* factory, bool detail);
static void on_chunk_free__(pool_factory_t* factory, size_t size);
static bool on_chunk_alloc__(pool_factory_t* factory, size_t size);
static int atp_binary_search(int value);


static void* pool_alloc_find(pool_t* pool, size_t size) {
	if (!pool || size <= 0) {
		LOG(ERROR) << "invalid args, pool or size is nil";
		return NULL;
	}

	pool_chunk_t* chunk_node = TAILQ_FIRST(&pool->chunk_list_);

	/* Traverse the chunk list */
	while (chunk_node != NULL) {
		mem = pool_alloc_from_chunk(chunk_node, size);
		if (mem != NULL) {
			return mem;
		}

		chunk_node = TAILQ_NEXT(chunk_node, next_);
	}

	/* In this, all nodes in pool chunk list not have enough memory, we create a new */


	if (pool->incr_size_ <= 0) {
		return NULL;
	}

	void* mem = NULL;
	void* new_chunk_size = pool->incr_size_;

	if (pool->incr_size_ < (size + sizeof(pool_chunk_t))) {
		int counts = (size + sizeof(pool_chunk_t) + pool->incr_size_ + ATP_MEMORY_POOL_ALIGN) / pool->incr_size_;
		new_chunk_size = pool->incr_size_ * counts;
	}

	chunk_node = create_chunk_append_pool(pool, new_chunk_size);
	if (!chunk_node) {
		return NULL;
	}

	/* Allocate memroy from new chunk */
	mem = pool_alloc_from_chunk(chunk_node, size);
	if (!mem) {
		return NULL;
	}

	return mem;
}

static void* pool_alloc_from_chunk(pool_chunk_t* chunk, size_t size) {
	if (!chunk || size <= 0) {
		LOG(ERROR) << "invalid args, chunk or size is nil";
		return NULL;
	}

	/* Whether or not nedd memory alignment */
	if (size & (ATP_MEMORY_POOL_ALIGN - 1)) {
		size = (size + ATP_MEMORY_POOL_ALIGN) & ~(ATP_MEMORY_POOL_ALIGN - 1);
	}

	size_t curr_chunk_size = size_t(chunk->end_ - chunk->cur_);
	if (curr_chunk_size >= size) {
		void* mem = chunk->cur_;
		chunk->cur_ += size;
		return mem;
	}

	/* The current doesn't have enough memory */
	return NULL;
}

static pool_chunk_t* create_chunk_append_pool(pool_t* pool, size_t chunk_size) {
	if (!pool) {
		return NULL;
	}

	assert(chunk_size >= sizeof(pool_chunk_t));

	pool_chunk_t* new_chunk = (pool_chunk_t*)(*pool->factory_->policy_.chunk_alloc)(pool->factory_, chunk_size);
	if (!new_chunk) {
		return NULL;
	}

	pool->capacity_ += chunk_size;

	new_chunk->buf_ = ((unsigned char*)new_chunk) + sizeof(pool_chunk_t);
	new_chunk->cur_ = ATP_ALIGN_PTR(new_chunk->buf_, ATP_MEMORY_POOL_ALIGN);
	new_chunk->end_ = ((unsigned char*)new_chunk) + chunk_size;

	/* Insert the new chunk to pool chunk list */
	TAILQ_INSERT_HEAD(&pool->chunk_list_, new_chunk, next_);

	return new_chunk;
}

void release_pool2(pool_t* pool) {
    assert(pool != NULL);

    pool_chunk_t* first_chunk = TAILQ_FIRST(&pool->chunk_list_);
    TAILQ_REMOVE(&pool->chunk_list_, first_chunk, entry);

    pool_chunk_t* tmp = NULL;

    TAILQ_FOREACH(tmp, &pool->chunk_list_, entry) {
        TAILQ_REMOVE(&pool->chunk_list_, tmp, entry);
        pool->factory_->policy.chunk_free(pool->factory, size_t(chunk->end_ - chunk->buf_));
    }

    pool->factory_->policy.chunk_free(pool->factory, size_t(first_chunk->end_ - (unsigned char*)pool));
}

size_t get_pool_capacity(pool_t* pool) {
	assert(pool != NULL);

	return pool->capacity_;
}

const char* get_pool_name(pool_t* pool) {
	assert(pool != NULL);

	return pool->name_;
}

pool_t* create_pool(pool_factory_t* factory, const char* name, size_t init_size, size_t incr_size) {
	return (*factory->create_pool)(factory, name, init_size, incr_size);
}

void release_pool(pool_t* pool, pool_factory_t* factory) {
	assert(pool != NULL);

	return (*factory->release_pool)(factory, pool);
}

void* pool_alloc(pool_t* pool, size_t size) {
	assert(pool != NULL);

	pool_chunk_t* chunk_node = TAILQ_FRIST(&pool->chunk_list_);
	void* mem = pool_alloc_from_chunk(chunk_node, size);
	if (!mem) {
		mem = pool_alloc_find(pool, size);
	}

	return mem;
}


pool_helper_t* create_pool_helper() {
	pool_helper_t* ph = (pool_helper_t*)calloc(1, sizeof(pool_helper_t));
	return ph;
}

void pool_helper_init(pool_helper_t* ph, const pool_factory_policy_t* policy, size_t max_capacity) {
	memset(ph, 0, sizeof(ph));
	ph->max_capacity_ = max_capacity;

	pthread_mutex_init(&ph->mutex_, NULL);

	TAILQ_INIT(&ph->used_pool_list_);
	for (int i = 0; i < ATP_MEMORY_POOL_CACHING_SIZE; ++ i) {
		TAILQ_INIT(&ph->used_pool_list_[i]);
	}

	/* If the policy is nil, then we used default policy */
	if (!policy) {
		policy = &default_policy;
	}

	memcpy(&ph->factory_.policy_, policy, sizeof(policy));

	ph->factory_.create_pool = &create_pool__;
	ph->factory_.release_pool = &release_pool__;
	ph->factory_.on_chunk_alloc = &on_chunk_alloc__;
	ph->factory_.on_chunk_free =  &on_chunk_free__;

}

void pool_helper_destroy(pool_helper* ph) {

}

size_t get_pool_helper_reference(pool_helper* ph) {
	return ph->used_count_;
}

size_t get_pool_helper_max_capacity(pool_helper* ph) {
	return ph->max_capacity_;
}

size_t get_pool_helper_used_mem_size(pool_helper* ph) {
	return ph->used_size_
}

pool_factory_t* get_pool_helper_factory(pool_helper* ph) {
	reutrn &ph->factory_;
}

static pool_t* create_pool__(pool_factory_t* factory, const char* name, size_t init_size, size_t incr_size) {
    ssize_t pos = binary_search(init_size);

    if (ATP_DEBUG_ON) {
        LOG(DEBUG) << "The binary search pos: " << pos << " init_size: " << init_size << " auto adjusted: " << pool_size_array[pos];
    }
                                                                                 
    pool_t* pool = NULL;
    pool_helper_t* ph = (pool_helper_t*)factory;

    /* The pool helper list operations is not thread safe, so we locked */
    pthread_mutex_lock(&ph->mutex);

    if (pos == ATP_MEMORY_POOL_CACHING_SIZE || TAILQ_EMPTY(&ph->free_pool_list_[i])) {
        if (pos < ATP_MEMORY_POOL_CACHING_SIZE) {
            init_size = pool_size_array[pos];
        }

        if (ATP_DEBUG_ON) {
            LOG(DEBUG) << "The 1 case init_size: " << init_size;
        }

        assert(init_size >= sizeof(pool_t) + sizeof(pool_chunk_t));
        unsigned char* mem = (unsigned char*)factory->policy_.chunk_alloc(factory, init_size);
        if (!mem) {
            pthread_mutex_unlock(&ph->mutex);
            LOG(ERROR) << "The factory policy chunk alloc failed";
            return NULL;
        }

        pool = (pool_t)mem;
        pool->factory_ = factory;
        pool->capacity_ = init_size;
        pool->incr_size_ = incr_size;
        strncpy(pool->name_, name, sizeof(name));
        TAILQ_INIT(&pool->chunk_list_);

        pool_chunk_t* chunk = (pool_chunk_t*)(mem + sizeof(pool_t));
        chunk->buf_ = ((unsigned char*)chunk) + sizeof(pool_chunk_t);
        chunk->cur_ = ATP_ALIGN_PTR(chunk->buf_, ATP_MEMORY_POOL_ALIGN);
        chunk->end_ = mem + init_size;

        TAILQ_INSERT(&pool->chunk_list_, chunk, entry);
    } else {
        pool = TAILQ_FIRST(&ph->free_pool_list_[i]);
        TAILQ_REMOVE(&ph->free_pool_list_[i], pool, entry);
        pool->incr_size = incr_size;
        strncpy(pool->name_, name, sizeof(name));

        if (ph->capacity_ > get_pool_capacity(pool)) {
            ph->capacity_ -= get_pool_capacity(pool);
        } else {
            ph->capacity_ = 0;
        }
    }

    pool->data_ = (void*)pos;
    TAILQ_INSERT_HEAD(&ph->used_pool_list_, pool, entry);
    ++ ph->used_count_;

    if (AST_DEBUG_ON) {
        LOG(DEBUG) << "The pool helper use count: " << ph->used_count_;
    }

    /* The pool helper unlock */
    pthread_mutex_unlock(&ph->mutex);

    return pool;
}

static void release_pool__(pool_factory_t* factory, pool_t* pool) {
	if (!factory || !pool) {
		return;
	}

	pool_helper_t* ph = (pool_helper_t*)factory;

	pthread_mutex_lock(&ph->mutex_);

	int exist = 0;
	pool_t* tmp = NULL;

	TAILQ_FOREACH(tmp, &ph->used_pool_list_, entry) {
		if (tmp == pool) {
			exist = 1;
			break;
		}
	}

	if (exist == 0) {
		pthread_mutex_unlock(&ph->mutex_);
        LOG(ERROR) << "The release pool: " << pool->name_ << " not in used_pool_list_";
		return;
	}

	TAILQ_REMOVE(&ph->used_pool_list_, pool, entry);
	-- ph->used_count_;

	ssize_t pos = (ssize_t)(void*)pool->data_;
	size_t cap = get_pool_capacity(pool);

	if (i == ATP_MEMORY_POOL_CACHING_SIZE ||
        (cap + ph->capacity_) > ph->max_capacity_ ||
        cap > pool_size_array[ATP_MEMORY_POOL_CACHING_SIZE - 1]) {
        release_pool2(pool);
        return;
	}

	TAILQ_INSERT_TAIL(&ph->free_pool_list_[pos], pool, entry);
	ph->capacity_ += cap;

	pthread_mutex_unlock(&ph->mutex_);
}

static void dump_status__(pool_factory_t* factory, bool detail) {
	/* Todo */
}

static void on_chunk_free__(pool_factory_t* factory, size_t size) {
	pool_helper_t* ph = (pool_helper_t*)factory;
	ph->used_size_ -= size;
}

static bool on_chunk_alloc__(pool_factory_t* factory, size_t size) {
	pool_helper_t* ph = (pool_helper_t*)factory;
	ph->used_count_ += size;
}

static int atp_binary_search(int value) {
	int pos = 0;
	int left = 0;
	int right = ATP_MEMORY_POOL_CACHING_SIZE - 1;
	int mid = 0;

    if (pool_size_array[left] >= value) {
        return left;
    }

    if (pool_size_array[right] < value) {
        return ATP_MEMORY_POOL_CACHING_SIZE;
    }

    while (left <= right) {
        mid = (left + right) / 2;
        if (pool_size_array[mid] >= value) {
            pos = mid;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

	return pos;
}

}/*end namespace atp*/
