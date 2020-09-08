/*
 * MIT License
 *
 * Copyright (c) 2019 pengwang7(https://github.com/pengwang7/libatp)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __ATP_THREAD_STORAGE_H__
#define __ATP_THREAD_STORAGE_H__

#include <stdlib.h>
#include <pthread.h>

#include "app/atp_api.h"

namespace atp {

/*
 * Data for thread locally storage variable
 */
struct threadstorage {
    pthread_key_t key;          /* The key used to retrieve this thread's data */
    pthread_once_t once;        /* Ensure that the key is only initialized by one thread */
    void (* key_init)();
    int  (* custom_init)(void*);
};

#define THREADSTORAGE(name) \
    THREADSTORAGE_CUSTOM_SCOPE(name, NULL, free, static)

#define THREADSTORAGE_CUSTOM_SCOPE(name, init, cleanup, scope)  \
    static void __init__ ## name();                 \
    scope struct threadstorage name = {             \
        .key = (pthread_key_t)0,                    \
        .once = PTHREAD_ONCE_INIT,                  \
        .key_init = __init__ ## name,               \
        .custom_init = init,                        \
    };                                              \
    static void __init__ ## name() {                \
        pthread_key_create(&(name).key, cleanup);   \
    }

INLINE_API(void* threadstorage_get(struct threadstorage* ts, size_t init_size),
{
    pthread_once(&ts->once, ts->key_init);

    void* buf = NULL;
    if (!(buf = pthread_getspecific(ts->key))) {
        if (!(buf = calloc(1, init_size))) {
            return NULL;
        }

        if (ts->custom_init && ts->custom_init(buf)) {
            free(buf);
            return NULL;
        }
        pthread_setspecific(ts->key, buf);
    }

    return buf;
})

INLINE_API(void* threadstorage_get_ptr(struct threadstorage* ts),
{
    pthread_once(&ts->once, ts->key_init);
    return pthread_getspecific(ts->key);
})

INLINE_API(int threadstorage_set_ptr(struct threadstorage* ts, void* ptr),
{
    pthread_once(&ts->once, ts->key_init);
    return pthread_setspecific(ts->key, ptr);
})

#define THREADSTORAGE_GET(ts, init_size) threadstorage_get(ts, init_size)

} /* end namespace atp */

#endif /* __ATP_THREAD_STORAGE_H__ */
