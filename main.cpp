#include <QCoreApplication>
#include "server.h"
#include <QDebug>
#include "redis.h"
#include <QByteArray>
#include <QDebug>
#include "json.h"
int main(int argc,char** argv)
{
    QCoreApplication app(argc,argv);

    new Server;
    /*QString str("main");
    QByteArray *array = new QByteArray;

    Redis::getInstance()->getList(str,array);
    Json j(array[0]);
    qDebug() << j.parse("name").toString();
    */
    return app.exec();
}
