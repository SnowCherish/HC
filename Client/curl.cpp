#include <string>
using namespace std;
#include <QDebug>
#include "curl.h"
#define HC_DOMAIN "127.0.0.1:9999"
Curl::Curl(QObject *parent) : QObject(parent)
{
    curl = curl_easy_init();
    if(curl==NULL)
    {
        qDebug() << "curl init error!";
        curl_global_cleanup();
        exit(1);
    }
    curl_easy_setopt(curl,CURLOPT_URL,HC_DOMAIN);
}
size_t read_back(void* buffer,size_t n,size_t mem,void* ptr)
{
    char* p = (char*)buffer;
    string& resp = *(string*)ptr;
    copy(p,p+n*mem,back_inserter(resp));
    return n*mem;
}
string Curl::request(char* ptr)
{
    CURLcode res;
    string resp;
    curl_easy_setopt(curl,CURLOPT_POST,1);
    curl_easy_setopt(curl,CURLOPT_POSTFIELDS,ptr);
    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,read_back);
    curl_easy_setopt(curl,CURLOPT_WRITEDATA,&resp);
    res = curl_easy_perform(curl);
    if(res)
    {
        qDebug() << "curl easy perform error!";
        exit(1);
    }
    return resp;
}

Curl::~Curl()
{
    if(curl!=NULL)
    {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        curl = NULL;
    }
}
