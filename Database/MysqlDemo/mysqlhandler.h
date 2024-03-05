// **********************************************************
// * Author        : wqj
// * Email         : 1107097641@qq.com
// * Create time   : 2022-02-24
// * Filename      : .cpp
// * Description   : 连接操作mysql数据库--需要重新编译qmysql驱动
// * Copyright     : (c) 2022 wqj
// **********************************************************
#ifndef MYSQLHANDLER_H
#define MYSQLHANDLER_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMutex>
#include <QDebug>

class MysqlHandler
{
public:
    MysqlHandler(const QString &host, const QString &databaseName, const QString &userName, const QString &password);

    //执行语句
    void executeCommand(const QString &command, const QVariantList &parameters = QVariantList());

    bool executeQuery(const QString &sql, QSqlQuery &sqlQuery);

    void close();

private:
    QString host;
    QString databaseName;
    QString userName;
    QString password;
    QSqlDatabase db;
    QMutex lock;
};

#endif // MYSQLHANDLER_H
