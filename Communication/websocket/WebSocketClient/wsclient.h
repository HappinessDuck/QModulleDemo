#ifndef WSCLIENT_H
#define WSCLIENT_H

#include <QObject>
#include <QWebSocket>
#include <QFile>
#include <QCoreApplication>

class WSClient : public QObject
{
    Q_OBJECT
public:
    WSClient();

    void init(QString &url,QString &UserId);
    //
    void onConnected();
    //
    void sendMessage(const QString &text,bool isText,QString sender,QString receiver);
    //
    void onSslErrors(const QList<QSslError> &errors);


    void dealText(QString text);
    void dealFile(QByteArray array);

signals:
    void showTextSignals(const QString &text);
    void showFileSignals(const QString &text);

private:
    QWebSocket m_webSocket;
    QFile m_file;
    qint64 m_fileSize;
    qint64 m_recvSize;
    bool isFile;

    QString execurrentPath;

    QString currentID;
};


#endif // WSCLIENT_H
