#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QFile>
#include <QMap>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    QMap<QString,QString> UserMap;  //连接队列用户信息
    bool isFile;

signals:


private slots:

    bool transferOrSave(const QString &text,bool isText,QString userName);

    void sendMessage(const QString &text,bool isText,QString recvUserName);

    void onError(QAbstractSocket::SocketError error);

    void on_pushButton_clicked();

private:
    Ui::Widget *ui;

//    void clearClient();

    QWebSocketServer *ws_server;
    QList<QWebSocket*> clientList;
//    QWebSocket *webSocket;

    int port;

    void getNewConnection();

    bool isFileExist(QString fullFileName);


    QFile m_file;
    qint64 m_fileSize;
    qint64 m_recvSize;

    int LinkSocketNum;



    QString sendUserName;
    QString recvUserName;



};

#endif // WIDGET_H
