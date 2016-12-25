#include "util.h"
#include <stdlib.h>
#include <time.h>
#include <QDateTime>
Util::Util(QObject *parent) : QObject(parent)
{

}
//geohash
qint64 Util::geohash(double lng, double lat,int bits)
{
    uint64_t lngLatBits = 0;

    double lngMin = -180;
    double lngMax = 180;
    double latMin = -90;
    double latMax = 90;


    for(int i=0; i<bits; ++i)
    {
        lngLatBits<<=1;

        double lngMid = (lngMax + lngMin)/2;
        if(lng > lngMid)
        {
            lngLatBits += 1;
            lngMin = lngMid;
        }
        else
        {
            lngMax = lngMid;
        }

        lngLatBits <<=1;

        double latMid = (latMax + latMin)/2;
        if(lat > latMid)
        {
            lngLatBits += 1;
            latMin = latMid;
        }
        else
        {
            latMax = latMid;
        }
    }
    return lngLatBits;
}
Util *Util::getInstance()
{
    static Util* util = NULL;
    if(util==NULL)
    {
        util = new Util;
    }
    return util;
}

QString Util::getCurrentTime()
{
    qint64 time = QDateTime::currentMSecsSinceEpoch();
    return QString::number(time);
}
QString Util::generId()
{
    srand((unsigned int )time(NULL));
    int i ;
    QString str;
    str = getCurrentTime();
    int res;
    for(i=0;i<15;++i)
    {
        res = rand()%10;
        str+=QString::number(res);
    }
    return str;
}
