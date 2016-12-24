#include <stdio.h>
#include <stdlib.h>
#include <QCoreApplication>
#include <QDateTime>
qint64 geohash(double Lat,double Lng,int bits)
{
    //qint64
}
QString getCurrentTime()
{
    QDateTime time = QDateTime::currentDateTime();
    return time.toLocalTime().toString();
}
