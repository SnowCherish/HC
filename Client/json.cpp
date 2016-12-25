#include "json.h"

Json* Json::json = NULL;
Json::Json(QObject *parent) : QObject(parent)
{
    root = cJSON_CreateObject();
}

Json::~Json()
{
    if(root!=NULL)
    {
        cJSON_Delete(root);
        root = NULL;
    }
    if(json!=NULL)
    {
        delete json;
        json = NULL;
    }
}

Json *Json::getInstance()
{
    if(json==NULL)
    {
        json = new Json();
    }
    return json;
}

Json* Json::insert(char *key, char *value)
{
    cJSON_AddItemToObject(root,key,cJSON_CreateString(value));
    return this;
}

Json* Json::insert(char *key, int value)
{
    cJSON_AddItemToObject(root,key,cJSON_CreateNumber(value));
    return this;
}

Json* Json::insert(char *key, double value)
{
    cJSON_AddItemToObject(root,key,cJSON_CreateNumber(value));
    return this;
}

char* Json::parseStr(char *key)
{
    return cJSON_GetObjectItem(root,key)->valuestring;
}

int Json::parseInt(char *key)
{
   return cJSON_GetObjectItem(root,key)->valueint;
}

double Json::parseDouble(char *key)
{
    return cJSON_GetObjectItem(root,key)->valuedouble;
}

char* Json::print()
{
    return cJSON_Print(root);
}
