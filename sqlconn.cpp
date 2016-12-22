#include "sqlconn.h"
#include <QDebug>
#include <stdlib.h>
#include "hcdef.h"
SqlConn::SqlConn(QObject *parent) : QObject(parent)
{
    sql = mysql_init(NULL);
    if(sql==NULL){
        qDebug() << "mysql init error!";
        exit(1);
    }
    sql = mysql_real_connect(sql,HC_ADDRESS,HC_USER,HC_PASSWD,HC_DB,0,NULL,0);
    if(sql==NULL){
        qDebug() << "mysql mysql_real_connect error!";
        exit(1);
    }
}

int SqlConn::insert(QString str)
{
    int status = mysql_query(sql,str.toUtf8().data());
    return status;
}

SqlConn::~SqlConn()
{
    if(sql!=NULL)
    {
        mysql_close(sql);
        sql = NULL;
    }
}

QString SqlConn::getErrInfo(int err)
{
    return QString(mysql_errno_to_sqlstate(err));
}
