#include <QCoreApplication>
#include "server.h"
#include "redis.h"
#include "sqlconn.h"
#include <QDebug>
#include "util.h"
int main(int argc,char** argv)
{
    QCoreApplication app(argc,argv);

    new Server;

    return app.exec();
}
