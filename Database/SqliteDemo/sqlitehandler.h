// **********************************************************
// * Author        : wqj
// * Email         : 1107097641@qq.com
// * Create time   : 2022-02-24
// * Filename      :
// * Description   : 连接操作sqlite数据库--无需额外编译添加驱动
// * Copyright     : (c) 2022 wqj
// **********************************************************
#ifndef SQLITEHANDLER_H
#define SQLITEHANDLER_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMutex>
#include <QDebug>
class SqliteHandler
{
public:
    SqliteHandler();

    bool open(const QString& dbName);
    bool executeQuery(const QString &sql, QSqlQuery &sqlQuery);
    void close();
private:
    QSqlDatabase db;
    QMutex lock;
};

#endif // SQLITEHANDLER_H
