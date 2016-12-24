#ifndef HCDEF_H
#define HCDEF_H
#include <mysql/mysql.h>
#include <mysql/mysql_com.h>

typedef MYSQL_ROW ROW;
#define HC_PORT 9999
#define HC_CMD "cmd"
#define HC_REG "reg"
#define HC_LOGIN "login"
#define HC_TYPE "type"
#define HC_DRIVER "driver"
#define HC_PASSENGER "passenger"
#define HC_ADDRESS "127.0.0.1"
#define HC_USER "root"
#define HC_PASSWD "123456"
#define HC_DB "hc"
#endif // HCDEF_H
