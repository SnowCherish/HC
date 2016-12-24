#include "server.h"
#include <QDebug>
#include <stdlib.h>
#include "json.h"
#include "hcdef.h"
#include "sqlconn.h"
#include "util.h"
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
 int Server::handle_Reg(QByteArray& req)
{
    Json j(req);
    QString type = j.parse(HC_TYPE).toString();
    QString id = "123123123";
    if(type.compare(HC_DRIVER)==0) //driver
    {


    }else if(type.compare(HC_PASSENGER)==0) //passenger
    {
        QString sql;
        //sql = QString("insert into passenger values('%1','%2','%3','%4','%5','%6','%7')").arg(id).arg(username)
          //      .arg(password).arg(age).arg(sex).arg(tel).arg(cardId);
        int ret = SqlConn::getInstance()->insert(sql);
        if(ret!=0)
        {
            qDebug() << "insert failed!";
            exit(1);
        }else{
            qDebug() << "insert ok!";
        }


    }
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
