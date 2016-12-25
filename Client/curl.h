#ifndef CURL_H
#define CURL_H

#include <QObject>
#include <curl/curl.h>
#include <string>
using namespace std;
#include "json.h"
class Curl : public QObject
{
    Q_OBJECT
public:
    explicit Curl(QObject *parent = 0);
    string request(char* ptr);
    ~Curl();
private:
    CURL* curl;
signals:

public slots:
};

#endif // CURL_H
