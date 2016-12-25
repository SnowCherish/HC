#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <tufao-1/Tufao/httpserver.h>
#include <tufao-1/Tufao/httpserverrequest.h>
#include <tufao-1/Tufao/httpserverresponse.h>
using namespace  Tufao;
class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    ~Server();
    void HandleReq(QByteArray req,HttpServerResponse& response);
    void HandleRes(HttpServerResponse& response);

private:
    HttpServer * server;
private:
     QByteArray handle_Login(QByteArray& req);
     QByteArray handle_Reg(QByteArray& req);
     void saveDriverStatus(int status,QString usrname,double lat,double lng,QString carId,QString tel,QString time);
     int IsExist(QString& sql);
signals:

public slots:
    void slotReadyReqRes(HttpServerRequest& request,HttpServerResponse& response);
};

#endif // SERVER_H
