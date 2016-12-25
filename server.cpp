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
int Server::IsExist(QString& sql)
{
    int ret = SqlConn::getInstance()->selData(sql,NULL);
    return ret;
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
QByteArray Server::handle_Login(QByteArray& req)
{
    int ret;
    Json j(req);
    QString type = j.parse(HC_TYPE).toString();
    QString username = j.parse("username").toString();
    QString passwd = j.parse("password").toString();
    QString password = j.encry(passwd);
    if(type==HC_DRIVER) //driver
    {
        QString sql;
        sql = QString("select * from driver where username=%1 and password=%2")
                .arg(username).arg(password);
        ret = IsExist(sql);
        if(ret!=1)
        {
            Json resp;
            resp.insert(HC_CMD,HC_LOGIN);
            resp.insert(HC_RESULT,HC_FAILED);
            resp.insert(HC_REASON,HC_NOTUSER);
            return resp.toJson();
        }
    }else if(type==HC_PASSENGER) //passenger
    {
        QString sql;
        sql = QString("select * from passenger where username=%1 and password=%2")
                .arg(username).arg(password);
        ret = IsExist(sql);
        if(ret!=1)
        {
            Json resp;
            resp.insert(HC_CMD,HC_LOGIN);
            resp.insert(HC_RESULT,HC_FAILED);
            resp.insert(HC_REASON,HC_NOTUSER);
            return resp.toJson();
        }
    }
    Json resp;
    resp.insert(HC_CMD,HC_LOGIN);
    resp.insert(HC_RESULT,HC_SUCCESS);
    return resp.toJson();
}

//handle reg
QByteArray Server::handle_Reg(QByteArray& req)
{
    int ret;
    Json j(req);
    QString type = j.parse(HC_TYPE).toString();
    QString id = Util::getInstance()->generId();
    QString username = j.parse("username").toString();
    QString sel;
    sel = QString("select * from passgenger where username=%1").arg(username);
    ret = IsExist(sel);
    if(ret>0) //username is already exists
    {
        Json resp;
        resp.insert(HC_CMD,HC_REG);
        resp.insert(HC_RESULT,HC_FAILED);
        resp.insert(HC_REASON,HC_USEREXIST);
        return resp.toJson();
    }
    QString passwd = j.parse("password").toString();
    QString password = j.encry(passwd);
    int age = j.parse("age").toInt();
    int sex = j.parse("sex").toInt();
    QString tel = j.parse("tel").toString();
    QString cardId = j.parse("cardId").toString();
    QString sql;
    if(type==HC_DRIVER) //driver
    {
        QString carId = j.parse("carId").toString();
        sql = QString("insert into driver values('%1','%2','%3','%4','%5','%6','%7','%8')").arg(id).arg(username)
                .arg(password).arg(age).arg(sex).arg(tel).arg(cardId).arg(carId);
        int ret = SqlConn::getInstance()->insert(sql);
        if(ret!=0)
        {
            Json resp;
            resp.insert(HC_CMD,HC_REG);
            resp.insert(HC_RESULT,HC_FAILED);
            resp.insert(HC_REASON,HC_REG_FAILED);
            return resp.toJson();
        }

    }else if(type==HC_PASSENGER) //passenger
    {

        sql = QString("insert into passenger values('%1','%2','%3','%4','%5','%6','%7')").arg(id).arg(username)
                .arg(password).arg(age).arg(sex).arg(tel).arg(cardId);
        int ret = SqlConn::getInstance()->insert(sql);
        if(ret!=0)
        {
            Json resp;
            resp.insert(HC_CMD,HC_REG);
            resp.insert(HC_RESULT,HC_FAILED);
            resp.insert(HC_REASON,HC_REG_FAILED);
            return resp.toJson();
        }

    }
    Json resp;
    resp.insert(HC_CMD,HC_REG);
    resp.insert(HC_RESULT,HC_SUCCESS);
    return resp.toJson();
}
void Server::HandleReq(QByteArray req,HttpServerResponse& response)
{
    Json j(req);
    QByteArray array;
    QString cmd = j.parse(HC_CMD).toString();
    if(cmd==HC_REG)
    {
        array = handle_Reg(req);
    }else if(cmd==HC_LOGIN)
    {
        array = handle_Login(req);
    }
    response.writeHead(HttpResponseStatus::OK);
    response.end(array);
}


void Server::slotReadyReqRes(HttpServerRequest& request,HttpServerResponse& response)
{
    HandleReq(request.readBody(),response);
}
