#include "redis.h"
#include "hcdef.h"
#include <QDebug>

Redis::Redis(QObject *parent) : QObject(parent)
{
    c = redisConnect(HC_ADDRESS,HC_REDIS_PORT);
    if(c->err)
    {
        redisFree(c);
        c = NULL;
        qDebug() << "redis connect err!";
        exit(1);
    }
}
Redis *Redis::getInstance()
{
    static Redis* redis = NULL;
    if(redis==NULL)
    {
        redis = new Redis;
    }
    return redis;
}

int Redis::set(QString &str, QByteArray &array)
{
    if(str.isEmpty() || array.isEmpty())
    {
        qDebug() << "str is NULL";
        return -1;
    }
    char buf[1024] = {0};
    sprintf(buf,"set %s %s",str.toUtf8().data(),array.data());
    r = (redisReply*)redisCommand(c,buf);
    if(r==NULL)
    {
        qDebug() << "execute command failed!";
        redisFree(c);
        return -1;
    }
    if(!((r->type == REDIS_REPLY_STATUS)&&(strcasecmp(r->str,"OK"))==0))
    {
        qDebug() << "execute command failed!";
        freeReplyObject(r);
        redisFree(c);
        return -1;
    }
    freeReplyObject(r);
    return 0;
}

int Redis::get(QByteArray& str)
{
    if(str==NULL)
    {
        qDebug() << "str is NULL";
        return -1;
    }
    r = (redisReply*)redisCommand(c,str.data());
    if(r->type!=REDIS_REPLY_STRING)
    {
        qDebug() << "failed to execute command";
        freeReplyObject(r);
        redisFree(c);
        return -1;
    }
    return 0;
}


Redis::~Redis()
{
    if(r!=NULL)
    {
        freeReplyObject(r);
        r = NULL;
    }
    if(c!=NULL)
    {
        redisFree(c);
        c = NULL;
    }
}
