#include <QCoreApplication>
#include "testserver.h"
//json
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TestServer testserver;
    testserver.initServer();

//    QJsonObject obj;
//    obj.insert("abc","123");
//    obj.insert("abc","");
//    qDebug()<<obj;



    return a.exec();
}
