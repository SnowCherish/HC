#ifndef HCDEF_H
#define HCDEF_H

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
#define HC_REDIS_PORT 6379
#define HC_RESULT "result"
#define HC_FAILED "failed"
#define HC_TIMEOUT "session time out"
#define HC_REASON "reason"
#define HC_USEREXIST "username is already exists"
#define HC_UNKNOW "unknow reason"
#define HC_SUCCESS "success"
#define HC_REG_FAILED "reg failed!"
#define HC_NOTUSER "user not exist"
#define HC_UPDATEPOS "updatepos"
#define HC_UPDATESTATUS "updatestatus"

typedef enum BOOL{FALSE=0,TRUE=1} Bool;

#endif // HCDEF_H
