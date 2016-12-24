#include <QCoreApplication>
#include "server.h"
#include "redis.h"
#include <QDebug>
int main(int argc,char** argv)
{
    QCoreApplication app(argc,argv);

    new Server;

    return app.exec();
}
