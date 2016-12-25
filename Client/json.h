#ifndef JSON_H
#define JSON_H

#include <QObject>
#include "cJSON.h"
class Json : public QObject
{
    Q_OBJECT
public:
    explicit Json(QObject *parent = 0);
    ~Json();
public:
    static Json* json;
    static Json* getInstance();
    Json* insert(char* key,char* value);
    Json* insert(char* key,int value);
    Json* insert(char* key,double value);
    char* parseStr(char* key);
    int parseInt(char *key);
    double parseDouble(char *key);
    char* print();
private:
    cJSON* root;
signals:

public slots:
};

#endif // JSON_H
