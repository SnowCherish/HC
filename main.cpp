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


    return app.exec();
}
