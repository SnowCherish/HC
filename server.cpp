#include "server.h"
#include <QDebug>
#include <stdlib.h>
#include "json.h"
#include "hcdef.h"
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
void Server::HandleReq(QByteArray req,HttpServerResponse& response)
{
    Json j(req);
    int ret = 0;
    QString cmd = j.parse(HC_CMD).toString();
    if(cmd==HC_REG)
    {

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
