#include <QCoreApplication>
#include "server.h"
#include <QDebug>
#include "redis.h"
#include <QByteArray>
#include <QDebug>
#include "json.h"
#include "util.h"
int main(int argc,char** argv)
{
    QCoreApplication app(argc,argv);

    new Server;
    QString username("123");
    QString lat("lng");
    QString data;
    Redis::getInstance()->getHash(username,lat,data);
    qDebug() << data.toDouble();
    return app.exec();
}
