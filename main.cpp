#include <QCoreApplication>
#include "server.h"
#include <QDebug>
#include "redis.h"
int main(int argc,char** argv)
{
    QCoreApplication app(argc,argv);

    new Server;

    return app.exec();
}
