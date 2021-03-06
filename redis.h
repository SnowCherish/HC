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
class Redis : public QObject
{
    Q_OBJECT
public:
    static Redis* getInstance();
    int get(QString& str,QByteArray& array);
    int set(QString& str,QByteArray& array);
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
