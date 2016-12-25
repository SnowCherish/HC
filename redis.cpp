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

//driver -->hash
int Redis::setDriverHash(QString &username, int& status,double lat,double lng,QString& geohash,QString& tel,
                         QString& carId,QString& time)
{
    if(username.isEmpty())
    {
        qDebug() << "str is NULL";
        return -1;
    }
    char buf[4096] = {0};
    sprintf(buf,"hmset %s status %d lat %f lng %f geohash %s tel %s carId %s time %s",
            username.toUtf8().data(),status,lat,lng,geohash.toUtf8().data(),tel.toUtf8().data(),
            carId.toUtf8().data(),time.toUtf8().data());
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
//passenger -->hash
int Redis::setPassHash(QString &username, double& lat,double& lng,QString& geohash,QString& tel,
                         QString& time)
{
    if(username.isEmpty())
    {
        qDebug() << "str is NULL";
        return -1;
    }
    char buf[4096] = {0};
    sprintf(buf,"hmset %s lat %f lng %f geohash %s tel %s time %s",
            username.toUtf8().data(),lat,lng,geohash.toUtf8().data(),tel.toUtf8().data(),
           time.toUtf8().data());
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

int Redis::setHash(QString &username, QString &str, QString &data)
{
    if(username.isEmpty())
    {
        qDebug() << "str is NULL";
        return -1;
    }
    char buf[4096] = {0};
    sprintf(buf,"hset %s %s %s",username.toUtf8().data(),str.toUtf8().data(),data.toUtf8().data());
    r = (redisReply*)redisCommand(c,buf);
    if(r==NULL)
    {
        qDebug() << "execute command failed!";
        redisFree(c);
        return -1;
    }
    if(!(r->type==REDIS_REPLY_INTEGER))
    {
        qDebug() << "execute command failed!";
        freeReplyObject(r);
        redisFree(c);
        return -1;
    }
    freeReplyObject(r);
    return 0;
}

int Redis::setHash(QString &username, QString &str, int &data)
{
    if(username.isEmpty())
    {
        qDebug() << "str is NULL";
        return -1;
    }
    char buf[4096] = {0};
    sprintf(buf,"hset %s %s %d",username.toUtf8().data(),str.toUtf8().data(),data);
    r = (redisReply*)redisCommand(c,buf);
    if(r==NULL)
    {
        qDebug() << "execute command failed!";
        redisFree(c);
        return -1;
    }
    if(!(r->type==REDIS_REPLY_INTEGER))
    {
        qDebug() << "execute command failed!";
        freeReplyObject(r);
        redisFree(c);
        return -1;
    }
    freeReplyObject(r);
    return 0;
}

int Redis::setHash(QString &username, QString &str, double &data)
{
    if(username.isEmpty())
    {
        qDebug() << "str is NULL";
        return -1;
    }
    char buf[4096] = {0};
    sprintf(buf,"hset %s %s %f",username.toUtf8().data(),str.toUtf8().data(),data);
    r = (redisReply*)redisCommand(c,buf);
    if(r==NULL)
    {
        qDebug() << "execute command failed!";
        redisFree(c);
        return -1;
    }
    if(!(r->type==REDIS_REPLY_INTEGER))
    {
        qDebug() << "execute command failed!";
        freeReplyObject(r);
        redisFree(c);
        return -1;
    }
    freeReplyObject(r);
    return 0;
}

//key--->value
int Redis::getHash(QString &username, QString &array,QString & data)
{
    if(username.isEmpty())
    {
        qDebug() << "str is NULL";
        return -1;
    }
    char buf[4096] = {0};
    sprintf(buf,"hget %s %s",username.toUtf8().data(),array.toUtf8().data());
    r = (redisReply*)redisCommand(c,buf);
    if(r==NULL)
    {
        qDebug() << "execute command failed!";
        redisFree(c);
        return -1;
    }
    if(!(r->type==REDIS_REPLY_STRING))
    {
        qDebug() << "execute command failed!";
        freeReplyObject(r);
        redisFree(c);
        return -1;
    }
    data = QString(r->str);
    freeReplyObject(r);
    return 0;
}

int Redis::setList(QString& str, QString& username)
{
    if(str.isEmpty())
    {
        qDebug() << "str is NULL";
        return -1;
    }
    char buf[1024] = {0};


    sprintf(buf,"lpush %s %s",str.toUtf8().data(),username.toUtf8().data());
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
