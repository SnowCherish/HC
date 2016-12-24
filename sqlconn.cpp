#include "sqlconn.h"
#include <QDebug>
#include <stdlib.h>
#include <mysql/mysql_com.h>
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

SqlConn *SqlConn::getInstance()
{
    static SqlConn* sqlConn = NULL;
    if(sqlConn==NULL)
    {
        sqlConn = new SqlConn;
    }
    return sqlConn;
}

int SqlConn::insert(QString str)
{
    int status = mysql_query(sql,str.toUtf8().data());
    if(status!=0)
    {
        qDebug() << "mysql query error!";
        exit(1);
    }
    return 0;
}

SqlConn::~SqlConn()
{
    if(sql!=NULL)
    {
        mysql_close(sql);
        sql = NULL;
    }
}

int SqlConn::selData(QString str,MYSQL_ROW& r)
{
    int status = mysql_query(sql,str.toUtf8().data());
    if(status!=0)
    {
        qDebug() << "mysql query error!";
        exit(1);
    }
    int count = mysql_field_count(sql);
    MYSQL_RES * result = mysql_store_result(sql);
    if(result==NULL)
    {
        qDebug() << "mysql store result error!";
        exit(1);
    }
    int i;
    MYSQL_ROW row ;

    while((row= mysql_fetch_row(result)))
    {
        for(i = 0;i<count;++i)
        {
            r[i] = row[i];
        }
    }
    mysql_free_result(result);
    return 0;
}

int SqlConn::update(QString str)
{
    int status  = mysql_query(sql,str.toUtf8().data());
    if(status!=0)
    {
        qDebug() << "mysql query error!";
        exit(1);
    }
    return 0;
}
