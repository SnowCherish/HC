#include "server.h"
#include <QDebug>
#include <stdlib.h>
#include "json.h"
#include "hcdef.h"
#include "sqlconn.h"
#include "util.h"
#include "redis.h"
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
//save driver status to redis
void Server::saveDriverStatus(int status,QString username,double lat,double lng,QString carId,QString tel,QString time)
{
    qint64 geohash = Util::getInstance()->geohash(lng,lat,20);

    QString key = QString::number(geohash);
    int ret = Redis::getInstance()->setList(key,username);
    if(ret!=0)
    {
        qDebug() << "save msg to redis error!";
        exit(1);
    }
    ret = Redis::getInstance()->setDriverHash(username,status,lat,lng,key,tel,carId,time);
    if(ret!=0)
    {
        qDebug() << "save msg to redis error!";
        exit(1);
    }
}
//save passenger status to redis
void Server::savePassStatus(QString username,double lat,double lng,QString tel,QString time)
{
    qint64 geohash = Util::getInstance()->geohash(lng,lat,20);
    QString key = QString::number(geohash);

    int ret = Redis::getInstance()->setPassHash(username,lat,lng,key,tel,time);
    if(ret!=0)
    {
        qDebug() << "save msg to redis error!";
        exit(1);
    }
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
//handle update driver's status
QByteArray handle_update_status(QByteArray& req)
{
    int ret;
    Json j(req);
    QString username = j.parse("username").toString();
    int status = j.parse("status").toInt();
    username+="D";
    ret = Redis::getInstance()->setHash(username,"status",status);
    if(ret!=0)
    {
        Json resp;
        resp.insert(HC_CMD,HC_UPDATESTATUS);
        resp.insert(HC_RESULT,HC_FAILED);
        resp.insert(HC_REASON,HC_TIMEOUT);
        return resp.toJson();
    }
    Json resp;
    resp.insert(HC_CMD,HC_UPDATESTATUS);
    resp.insert(HC_RESULT,HC_SUCCESS);
    return resp.toJson();
}
//handle update pos
QByteArray Server::handle_updatepos(QByteArray& req)
{
    int ret;
    Json j(req);
    QString type = j.parse("type").toString();
    QString username = j.parse("username").toString();
    double lat = j.parse("lat").toDouble();
    double lng = j.parse("lng").toDouble();
    qint64 geohash = Util::getInstance()->geohash(lng,lat,20);
    QString hash = QString::number(geohash);
    if(type==HC_DRIVER)
    {
        username+="D";
    }else if(type==HC_PASSENGER)
    {
        username+="P";
    }
    ret = Redis::getInstance()->setHash(username,"lat",lat);
    if(ret!=0)
    {
        Json resp;
        resp.insert(HC_CMD,HC_UPDATEPOS);
        resp.insert(HC_RESULT,HC_FAILED);
        resp.insert(HC_REASON,HC_TIMEOUT);
        return resp.toJson();
    }
    ret = Redis::getInstance()->setHash(username,"lng",lng);
    if(ret!=0)
    {
        Json resp;
        resp.insert(HC_CMD,HC_UPDATEPOS);
        resp.insert(HC_RESULT,HC_FAILED);
        resp.insert(HC_REASON,HC_TIMEOUT);
        return resp.toJson();
    }
    ret = Redis::getInstance()->setHash(username,"geohash",hash);
    if(ret!=0)
    {
        Json resp;
        resp.insert(HC_CMD,HC_UPDATEPOS);
        resp.insert(HC_RESULT,HC_FAILED);
        resp.insert(HC_REASON,HC_TIMEOUT);
        return resp.toJson();
    }
    Json resp;
    resp.insert(HC_CMD,HC_UPDATEPOS);
    resp.insert(HC_RESULT,HC_SUCCESS);
    return resp.toJson();
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
    double lat = j.parse("lat").toDouble();
    double lng = j.parse("lng").toDouble();
    QString time = Util::getInstance()->getCurrentTime();
    if(type==HC_DRIVER) //driver
    {
        username+="D";
        QString sql;
        sql = QString("select * from driver where username='%1' and password='%2'")
                .arg(username).arg(password);
        QByteArray * array = new QByteArray;
        ret = SqlConn::getInstance()->selData(sql,array);
        if(ret!=1)
        {
            Json resp;
            resp.insert(HC_CMD,HC_LOGIN);
            resp.insert(HC_RESULT,HC_FAILED);
            resp.insert(HC_REASON,HC_NOTUSER);
            return resp.toJson();
        }
        Json msg(array[0]);
        QString carId = msg.parse("carid").toString();
        QString tel = msg.parse("tel").toString();
        saveDriverStatus(1,username,lat,lng,carId,tel,time);
        delete array;
    }else if(type==HC_PASSENGER) //passenger
    {
        username+="P";
        QString sql;
        sql = QString("select * from passenger where username='%1' and password='%2'")
                .arg(username).arg(password);
        QByteArray * array = new QByteArray;
        ret = SqlConn::getInstance()->selData(sql,array);
        if(ret!=1)
        {
            Json resp;
            resp.insert(HC_CMD,HC_LOGIN);
            resp.insert(HC_RESULT,HC_FAILED);
            resp.insert(HC_REASON,HC_NOTUSER);
            return resp.toJson();
        }
        Json msg(array[0]);
        QString tel = msg.parse("tel").toString();
        savePassStatus(username,lat,lng,tel,time);
        delete array;
    }
    Json resp;
    resp.insert(HC_CMD,HC_LOGIN);
    resp.insert(HC_RESULT,HC_SUCCESS);
    return resp.toJson();
}

//handle reg
QByteArray Server::handle_Reg(QByteArray& req)
{

    Json j(req);
    QString type = j.parse(HC_TYPE).toString();
    QString id = Util::getInstance()->generId();
    QString username = j.parse("username").toString();

    QString passwd = j.parse("password").toString();
    QString password = j.encry(passwd);
    int age = j.parse("age").toInt();
    int sex = j.parse("sex").toInt();
    QString tel = j.parse("tel").toString();
    QString cardId = j.parse("cardId").toString();
    QString sql;
    if(type==HC_DRIVER) //driver
    {
        username+="D";
        QString sel;
        sel = QString("select count(*) from driver where username='%1'").arg(username);
        QByteArray* array = new QByteArray;
        SqlConn::getInstance()->selData(sel,array);
        Json json(array[0]);
        QString res = json.parse("count(*)").toString();
        delete array;
        if(res!="0") //username is already exists
        {
            Json resp;
            resp.insert(HC_CMD,HC_REG);
            resp.insert(HC_RESULT,HC_FAILED);
            resp.insert(HC_REASON,HC_USEREXIST);
            return resp.toJson();
        }
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
        username+="P";
        QString sel;
        sel = QString("select count(*) from passenger where username='%1'").arg(username);
        QByteArray* array = new QByteArray;
        SqlConn::getInstance()->selData(sel,array);
        Json j(array[0]);
        QString res = j.parse("count(*)").toString();
        delete array;
        if(res!="0") //username is already exists
        {
            Json resp;
            resp.insert(HC_CMD,HC_REG);
            resp.insert(HC_RESULT,HC_FAILED);
            resp.insert(HC_REASON,HC_USEREXIST);
            return resp.toJson();
        }
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
//handle request
void Server::HandleReq(QByteArray req,HttpServerResponse& response)
{
    Json j(req);
    QByteArray array;
    QString cmd = j.parse(HC_CMD).toString();
    if(cmd==HC_UPDATEPOS)
    {
        array = handle_updatepos(req);
    }
    else if(cmd==HC_REG)
    {
        array = handle_Reg(req);
    }else if(cmd==HC_LOGIN)
    {
        array = handle_Login(req);
    }else if(cmd==HC_UPDATESTATUS)
    {
        array = handle_update_status(req);
    }
    response.writeHead(HttpResponseStatus::OK);
    response.end(array);
}


void Server::slotReadyReqRes(HttpServerRequest& request,HttpServerResponse& response)
{
    HandleReq(request.readBody(),response);
}
