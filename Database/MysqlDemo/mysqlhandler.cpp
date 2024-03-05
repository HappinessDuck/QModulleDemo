#include "mysqlhandler.h"


MysqlHandler::MysqlHandler(const QString &host, const QString &databaseName, const QString &userName, const QString &password)
    : host(host), databaseName(databaseName), userName(userName), password(password)
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(host);
    db.setDatabaseName(databaseName);
    db.setUserName(userName);
    db.setPassword(password);
    db.setPort(3306);

    if (!db.open()) {
        qWarning() << "Failed to open database:" << db.lastError().text();
    }
}

void MysqlHandler::executeCommand(const QString &command, const QVariantList &parameters)
{
    QMutexLocker locker(&lock);
    QSqlQuery query(db);

    query.prepare(command);
    for (const auto &param : parameters) {
        query.addBindValue(param);
    }

    if (!query.exec()) {
        qWarning() << "Error executing command:" << query.lastError().text();
    }
}

bool MysqlHandler::executeQuery(const QString &sql, QSqlQuery &sqlQuery)
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


void MysqlHandler::close()
{
    db.close();
}
