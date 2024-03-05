#include "sqlitehandler.h"


SqliteHandler::SqliteHandler()
{

}

bool SqliteHandler::open(const QString& dbName)
{
    if(QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbName); // 需要包含路径
    }

    if(!db.open())
    {
        qDebug()<<"error: failed to connect sqlite3 database."<< db.lastError();
        return false;
    }
    else {
        qDebug()<<"success to connect sqlite3 database.";
        return true;
    }

}

bool SqliteHandler::executeQuery(const QString &sql, QSqlQuery &sqlQuery)
{
    QMutexLocker locker(&lock);
    try {
        if (!sqlQuery.prepare(sql))
        {
            qDebug() << "Error preparing query: " << sqlQuery.lastError().text();
            return false;
        }
        if (!sqlQuery.exec())
        {
            qDebug() << "Error executing query: " << sqlQuery.lastError().text();
            return false;
        }
    } catch (const std::exception& e)
    {
        qDebug() << "Exception occurred: " << e.what();
    }
    return true;
}

void SqliteHandler::close()
{
    db.close();
}
