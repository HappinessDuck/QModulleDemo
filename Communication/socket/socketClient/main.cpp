#include <QCoreApplication>
#include "testclient.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TestClient testclient;
    testclient.initClient();
//    testclient.test();

    return a.exec();
}
