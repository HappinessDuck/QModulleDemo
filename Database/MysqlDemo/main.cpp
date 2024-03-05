#include <QCoreApplication>
#include "mysqlhandler.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MysqlHandler handler("localhost", "sport_result", "root", "123456");

    QString table_name = "student_msg";
    int student_id = 88;
    QString student_name = "www";
    QString class_name = "222";
    QString photo_url = "www.baidu.com";


    QString sql = QString("INSERT INTO %1 (student_id, student_name, class_name, photo_url) VALUES(?, ?, ?, ?)").arg(table_name);

    // Execute a SQL command
//    handler.executeCommand(sql, {student_id, student_name, class_name, photo_url});


    // Execute a SQL query
    sql = QString("SELECT * FROM %1 WHERE student_id = %2").arg(table_name).arg(4);


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
