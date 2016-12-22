#ifndef SQLCONN_H
#define SQLCONN_H

#include <QObject>
#include <mysql/mysql.h>
class SqlConn : public QObject
{
    Q_OBJECT
public:
    explicit SqlConn(QObject *parent = 0);
    int insert(QString sql);
    ~SqlConn();
private:
    MYSQL* sql;

signals:

public slots:
};

#endif // SQLCONN_H
