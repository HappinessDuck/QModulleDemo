#include <QCoreApplication>
#include "sqlitehandler.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    SqliteHandler handler;
    handler.open("E:/study/cPlus/QModuleDemo/QModulleDemo/Database/SqliteDemo/SportData.db");

    QString table_name = "student_msg";
    QString sql = QString("SELECT * FROM %1 WHERE student_id = %2").arg(table_name).arg(4);

    QSqlQuery sqlQuery;
    if (handler.executeQuery(sql, sqlQuery))
    {
        while(sqlQuery.next())
        {
            qDebug()<<sqlQuery.value(0).toString();
            qDebug()<<sqlQuery.value(1).toString();
            qDebug()<<sqlQuery.value(2).toString();
            qDebug()<<sqlQuery.value(3).toString();
            qDebug()<<sqlQuery.value("photo_url").toString();
        }
    }

    return a.exec();
}
