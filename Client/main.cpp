#include <QCoreApplication>
#include <string>
using namespace std;
#include "json.h"
#include <QDebug>
#include "curl.h"
int main(int argc,char** argv)
{
    QCoreApplication app(argc,argv);

    Curl c;
    Json j;
    j.insert("cmd","reg");
    j.insert("type","driver");
    j.insert("username","snow");
    j.insert("password","123456");
    j.insert("age",18);
    j.insert("sex",1);
    j.insert("carId","qwerdfsdadasd");
    j.insert("tel","1231231231");
    j.insert("cardId","3123012031900834");
    string buf = c.request(j.print());
    qDebug() << buf.c_str();
    return app.exec();
}
