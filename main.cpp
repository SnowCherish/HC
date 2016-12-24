#include <QCoreApplication>
#include "server.h"
#include "redis.h"
#include "sqlconn.h"
#include <QDebug>
#include "hcdef.h"
#include "util.h"
#include <QByteArray>
#include "json.h"
int main(int argc,char** argv)
{
    QCoreApplication app(argc,argv);

    new Server;
    QString sql("select * from passenger");
    QByteArray *array = new QByteArray;
    int ret = SqlConn::getInstance()->selData(sql,array);
    qDebug() << ret;
    //qDebug() << array[0].data();
    Json j(array[0]);
    qDebug() << j.parse("age").toString();
    Json j2(array[1]);
    qDebug() << j2.parse("age").toString();
    //qDebug() << array[1].data();
    return app.exec();
}
