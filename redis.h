#ifndef REDIS_H
#define REDIS_H

#include <QObject>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <hiredis/hiredis.h>
#include <QByteArray>
#include <QString>
#include <QVector>
class Redis : public QObject
{
    Q_OBJECT
public:
    static Redis* redis;
    static Redis* getInstance();
    int get(QString& str,QByteArray& array);
    int set(QString& str,QByteArray array);
    int setList(QString& str,QString& username);
    int setDriverHash(QString &username, int& status,double lat,double lng,QString& geohash,QString& tel,
                             QString& carId,QString& time);
    int getHash(QString& username,QString& array,QString & data);
    int setPassHash(QString &username, double& lat,double& lng,QString& geohash,QString& tel,
                             QString& time);
    int getList(QString& str,QVector<QString>& list);
    int setHash(QString& username,QString& str,QString& data);
    int setHash(QString& username,QString& str,int& data);
    int setHash(QString& username,QString& str,double& data);
    int del(QString& key);
    int removeFromList(QString& key,QString& username);
private:
    explicit Redis(QObject *parent = 0);
    ~Redis();
private:
    redisReply * r;
    redisContext* c;
signals:

public slots:
};

#endif // REDIS_H
