#include "redis.h"
#include "hcdef.h"
#include <QDebug>
#include <string.h>
#include "json.h"

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

int Redis::set(QString &str, QByteArray array)
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

int Redis::setList(QString& str, QByteArray array)
{
    if(str.isEmpty() || array.isEmpty())
    {
        qDebug() << "str is NULL";
        return -1;
    }
    char buf[1024] = {0};


    sprintf(buf,"lpush %s %s",str.toUtf8().data(),array.toBase64().data());
    //qDebug() << buf;
    r = (redisReply*)redisCommand(c,buf);
    if(r==NULL)
    {
        qDebug() << "execute command failed!";
        redisFree(c);
        return -1;
    }
    if(!(r->type == REDIS_REPLY_INTEGER))
    {
        qDebug() << "REDIS_REPLY_INTEGER failed!";
        freeReplyObject(r);
        redisFree(c);
        return -1;
    }
    freeReplyObject(r);
    return 0;
}

int Redis::getList(QString str, QByteArray *array)
{

    if(str==NULL)
    {
        qDebug() << "str is NULL";
        return -1;
    }
    char buf[1024] = {0};
    sprintf(buf,"lrange %s 0 -1",str.toUtf8().data());
    r = (redisReply*)redisCommand(c,buf);
    if(r->type!=REDIS_REPLY_ARRAY)
    {
        qDebug() << "failed to execute command";
        freeReplyObject(r);
        redisFree(c);
        return -1;
    }
    size_t i;
    redisReply* reply;
    for(i=0;i<r->elements;++i){
        reply= r->element[i];

        array->push_back(QByteArray::fromBase64(reply->str));
    }
    freeReplyObject(r);
    return 0;
}

int Redis::get(QString& str,QByteArray& array)
{

    if(str==NULL)
    {
        qDebug() << "str is NULL";
        return -1;
    }
    char buf[1024] = {0};
    sprintf(buf,"get %s ",str.toUtf8().data());
    r = (redisReply*)redisCommand(c,buf);
    if(r->type!=REDIS_REPLY_STRING)
    {
        qDebug() << "failed to execute command";
        freeReplyObject(r);
        redisFree(c);
        return -1;
    }
    array = QString(r->str).toUtf8();
    freeReplyObject(r);
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
