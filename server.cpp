#include "server.h"
#include <QDebug>
#include <stdlib.h>
#include "json.h"
#include "hcdef.h"
#include "sqlconn.h"
#include "util.h"

static QString str="";//save error info
Server::Server(QObject *parent) : QObject(parent)
{
    server = new HttpServer();
    server->listen(QHostAddress::Any,HC_PORT);
    if(!server->isListening())
    {
        qDebug() <<"server isn't listening!";
        exit(1);
    }
    qDebug() << "Server is listening!";
    connect(server,&HttpServer::requestReady,this,&Server::slotReadyReqRes);
}

Server::~Server()
{
    if(server!=NULL)
    {
        server->close();
        server = NULL;
    }
}
//handle login
int Server::handle_Login(QByteArray& req)
{
    Json j(req);
    QString type = j.parse(HC_TYPE).toString();
    if(type.compare(HC_DRIVER)==0) //driver
    {

    }else if(type.compare(HC_PASSENGER)==0) //passenger
    {

    }
}
int Server::IsExist(QString name)
{
    QString sql;
    sql = QString("select * from passgenger where username=%1").arg(name);
}
//handle reg
int Server::handle_Reg(QByteArray& req)
{
    Json j(req);
    QString type = j.parse(HC_TYPE).toString();
    QString id = Util::getInstance()->generId();
    if(type.compare(HC_DRIVER)==0) //driver
    {


    }else if(type.compare(HC_PASSENGER)==0) //passenger
    {
        QString username = j.parse("username").toString();
        QString password = j.parse("password").toString();
        int age = j.parse("age").toInt();
        int sex = j.parse("sex").toInt();
        QString tel = j.parse("tel").toString();
        QString cardId = j.parse("cardId").toString();
        QString sql;
        IsExist(username);
        sql = QString("insert into passenger values('%1','%2','%3','%4','%5','%6','%7')").arg(id).arg(username)
                .arg(password).arg(age).arg(sex).arg(tel).arg(cardId);
        int ret = SqlConn::getInstance()->insert(sql);
        if(ret!=0)
        {
            qDebug() << "passenger reg failed!";
            return -1;

        }
        qDebug() << "passenger reg ok!";
    }
    return 0;
}
void Server::HandleReq(QByteArray req,HttpServerResponse& response)
{
    Json j(req);
    int ret = 0;
    QString cmd = j.parse(HC_CMD).toString();
    if(cmd==HC_REG)
    {
        ret = handle_Reg(req);
    }else if(cmd==HC_LOGIN)
    {
        ret = handle_Login(req);
    }

}


void Server::slotReadyReqRes(HttpServerRequest& request,HttpServerResponse& response)
{
    //handle request
    HandleReq(request.readBody(),response);
    //handle response
    //HandleRes(response);
}
