#ifndef __ATP_DAO_H__
#define __ATP_DAO_H__

#include <string>
#include <vector>
#include <unordered_map>

#include "hiredis/async.h"

namespace atp {

/* The redis sync interface. */
redisContext* redis_connect(const char* host, const unsigned int port);

int redis_close(redisContext* c);

int redis_expire(redisContext* c, const char* key, const unsigned int sec);
			
int redis_multi(redisContext* c);

int redis_discard(redisContext* c);

int redis_exec(redisContext* c);

int redis_set(redisContext* c, const char* key, const char* val);

int redis_get(redisContext* c, const char* key, char* ret);

int redis_del(redisContext* c, const char* key);
			
int redis_hset(redisContext* c, const char* key , const char* filed, const char* value);

int redis_hget(redisContext* c, const char* key, const char* filed, char* ret);

int redis_hdel(redisContext* c, const char* key, const char* filed);

std::vector<std::string> redis_hgetall(redisContext* c, const char* key);
			
std::unordered_map<std::string, std::string> redis_hgetall_2(redisContext* c, const char* key);

int redis_lpush(redisContext* c, const char* key, const char* value);

int redis_lpop(redisContext* c, const char* key, char* res);

int redis_rpush(redisContext* c, const char* key , const char* value);

int redis_rpop(redisContext* c, const char* key, char* res);

int redis_lrem(redisContext* c, const char* key, int top, const char* value);

std::vector<std::string> redis_lrange(redisContext* c, const char* key, int begin, int end); 

} /* end namespace atp */

#endif /* __ATP_DAO_H__ */
