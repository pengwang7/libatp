#include <string.h>

#include "atp_dao.h"
#include "atp_debug.h"
//#include "glog/logging.h"

namespace atp {

redisContext* redis_connect(const char* host, const unsigned int port) {
    redisContext* c = redisConnect(host, port);
    if (!c || c->err) {
        return NULL;
    }

    return c;
}

int redis_close(redisContext* c) {
    int ret = -1;

    if (NULL != c) {
        ret = 0;
        redisFree(c);
    }

    return ret;
}

int redis_expire(redisContext* c, const char* key, const unsigned int sec) {
    int ret = -1;

    assert(c);

    redisReply* reply = (redisReply*)(redisCommand(c, "EXPIRE %s %u", key, sec));

    if (NULL != reply && reply->type == REDIS_REPLY_STATUS) {
        ret = 0;	
    }

    if (reply != NULL) {
        freeReplyObject(reply);
    }

    return ret;
}
			
int redis_multi(redisContext* c) {
    int ret = -1;

    assert(c);

    redisReply* reply = (redisReply*)(redisCommand(c, "MULTI"));

    if (NULL != reply
        && NULL != reply->str
        && reply->type == REDIS_REPLY_STATUS
        && strcmp(reply->str, "OK") == 0) {
        ret = 0;
    }

    if (NULL != reply) {
        freeReplyObject(reply);
    }

    return ret;
}

int redis_discard(redisContext* c) {
    int ret = -1;

    assert(c);

    redisReply* reply = (redisReply*)(redisCommand(c, "DISCARD"));

    if (NULL != reply
        && NULL != reply->str
        && reply->type == REDIS_REPLY_STATUS 
        && strcasecmp(reply->str, "OK") == 0) {
        ret = 0;
    }

    freeReplyObject(reply);

    return ret;
}

int redis_exec(redisContext* c) {
    int ret = -1;

    assert(c);

    redisReply* reply = (redisReply*)(redisCommand(c, "EXEC"));
    //不允许multi exec之间一条命令都不写
    if (NULL != reply
        && reply->elements != 0
        && reply->type == REDIS_REPLY_ARRAY) {
        ret = 0;
    }

    if (NULL != reply) {
        freeReplyObject(reply);
    }

    return ret;
}

int redis_set(redisContext* c, const char* key, const char* val) {
    int ret = -1;

    assert(c);

	redisReply* reply = (redisReply*)(redisCommand(c, "SET %s %s", key, val));
    if (NULL != reply
        && NULL != reply->str
        && reply->type == REDIS_REPLY_STATUS
        && (strcmp( reply->str, "OK")
        && strcmp(reply->str, "QUEUED")) == 0) {
        ret = 0;
    }

    if (NULL != reply) {
        freeReplyObject(reply);
    }

    return ret;
}

int redis_get(redisContext* c, const char* key, char* res) {
    int ret = -1;

    assert(c);

    redisReply* reply = (redisReply*)(redisCommand(c, "GET %s", key));
    if (NULL != reply 
        && reply->str != NULL 
        && reply->type == REDIS_REPLY_STRING) {
        ret = 0;
        memcpy(res, reply->str, reply->len);
    }
	
    if (reply != NULL) {
        freeReplyObject(reply);
    }

    return ret;
}

int redis_del(redisContext* c, const char* key) {
    int ret = -1;

    assert(c);

	redisReply* reply = (redisReply*)(redisCommand(c, "DEL %s", key));

    if (NULL != reply
        && reply->type == REDIS_REPLY_INTEGER
        && reply->integer == 1) {
        ret = 0;
    }

    if (NULL != reply) {
        freeReplyObject(reply);
    }

    return ret;
}
		
int redis_hset(redisContext* c, const char* key , const char* filed, const char* value) {
    int ret = -1;

    assert(c);

    redisReply* reply = (redisReply*)(redisCommand(c, "HSET %s %s %s", key, filed, value));

    if (NULL != reply
        && reply->type == REDIS_REPLY_STATUS) {
        ret = 0;
    }

    if (NULL != reply) {
        freeReplyObject(reply);
    }

    return ret;
}

int redis_hget(redisContext* c, const char* key, const char* filed, char* res) {
    int ret = 0;

    assert(c);

    redisReply* reply = (redisReply*)(redisCommand(c, "HGET %s %s", key, filed));

    if (NULL != reply
        && reply->type == REDIS_REPLY_STRING
        && reply->str != NULL) {
        ret = 0;
        memcpy(res, reply->str, reply->len);
    }

    if (NULL != reply) {
        freeReplyObject(reply);
    }

    return ret;
}

int redis_hdel(redisContext* c, const char* key, const char* filed) {
    int ret = 0;

    assert(c);

    redisReply* reply = (redisReply*)(redisCommand(c, "HDEL %s %s", key, filed));

    if (NULL != reply
        && reply->type == REDIS_REPLY_INTEGER) {
        ret = 0;
    }

    if (NULL != c) {
        freeReplyObject(c);
    }

    return ret;
}

std::vector<std::string> redis_hgetall(redisContext* c, const char* key) {
    std::vector< std::string > ret;

    assert(c);

    redisReply* reply = (redisReply*)(redisCommand(c, "HGETALL %s", key));
    if (NULL != reply
        && reply->type == REDIS_REPLY_ARRAY
        && reply->elements != 0) {

        for (std::size_t i = 0; i < reply->elements; ++ i) {
            std::string tmp;
            tmp.assign(reply->element[i]->str, reply->element[i]->len);
            ret.push_back(tmp);
        }
    }

    if (NULL != reply) {
        freeReplyObject(reply);
    }

    return ret;
}

std::unordered_map<std::string, std::string>
redis_hgetall_2(redisContext* c, const char* key) {
    std::unordered_map< std::string, std::string > ret;

    assert(c);

    redisReply* reply = (redisReply*)(redisCommand(c, "HGETALL %s", key));

    if (NULL != reply
        && reply->type == REDIS_REPLY_ARRAY
        && reply->elements != 0) {

        for (std::size_t i = 0; i < reply->elements - 1; i += 2) {
            std::string key;
            std::string val;
            key.assign(reply->element[i]->str, reply->element[i]->len);
            val.assign(reply->element[i + 1]->str, reply->element[i + 1]->len); 

            ret[key] = val;
        }
    }

    if (NULL != reply) {
        freeReplyObject(reply);
    }

    return ret;
}

int redis_lpush(redisContext* c, const char* key, const char* value) {
    int ret = -1;

    assert(c);

    redisReply* reply = (redisReply*)(redisCommand(c, "LPUSH %s %s", key, value));

    if (NULL != reply
        && reply->type == REDIS_REPLY_INTEGER) {
        ret = 0;
    }

    if (NULL != reply) {
        freeReplyObject(reply);
    }

    return ret;
}

int redis_lpop(redisContext* c, const char* key, char* res) {
	int ret = -1;

	assert(c);

    redisReply* reply = (redisReply*)(redisCommand(c, "LPOP %s ", key));
	if (NULL != reply
        && reply->type == REDIS_REPLY_STRING
        && reply->str != NULL) {
        ret = 0;
        memcpy(res, reply->str, reply->len);
    }

    if (NULL != reply) {
        freeReplyObject(reply);
    }

    return ret;
}

int redis_rpush(redisContext* c, const char* key, const char* value) {
	int ret = -1;

	assert(c);
	
	redisReply* reply = (redisReply*) (
		redisCommand(c, "RPUSH %s %s", key, value));
		
	if (NULL != reply 
		&& reply->type == REDIS_REPLY_INTEGER) {
		ret = 0;
	}
	
	if (NULL != reply) {
		freeReplyObject(reply);
	}
	
	return ret;
}

int redis_rpop(redisContext* c, const char* key , char* res) {
    int ret = -1;

	assert(c);

    redisReply* reply = (redisReply*)(redisCommand(c, "RPOP %s ", key));
	if (NULL != reply
        && reply->type == REDIS_REPLY_STRING
        && reply->str != NULL) {
        ret = 0;
        memcpy(res, reply->str, reply->len);
    }

    if (NULL != reply) {
        freeReplyObject(reply);
    }

    return ret;
}

int redis_lrem(redisContext* c, const char* key, int top, const char* value) {
    int ret = -1;

    assert(c);

    redisReply* reply = (redisReply*)(redisCommand(c, "LREM %s %d %s", key, top, value));
    if (NULL != reply
        && reply->type == REDIS_REPLY_INTEGER) {
        ret = 0;
    }

    if (NULL != reply) {
        freeReplyObject(reply);
    }

    return ret;
}

std::vector<std::string> redis_lrange(redisContext* c , const char* key, int begin, int end) {
    std::vector< std::string > ret;

    assert(c);

    redisReply* reply = (redisReply*)(redisCommand(c, "LRANGE %s %d %d", key, begin, end));
	if (NULL != reply
        && reply->type == REDIS_REPLY_ARRAY
        && reply->elements != 0) {

        for (size_t i = 0; i < reply->elements; ++ i) {
            std::string tmp;
            tmp.assign(reply->element[i]->str, reply->element[i]->len);
            ret.push_back(tmp);
		}
	}

    if (NULL != reply) {
        freeReplyObject(reply);
    }
	
    return ret;
}

} /* end namespace atp */
