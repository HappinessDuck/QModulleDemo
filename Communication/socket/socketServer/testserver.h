#ifndef TESTSERVER_H
#define TESTSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QRunnable>
#include <QThreadPool>

#include <QRunnable>
class TestServer: public QObject
{
    Q_OBJECT    //添加这个宏是为了实现信号和槽的通信
public:
    TestServer();

    void initServer();

private slots:
    //读取信息
    void ReadData();
    //新建连接
    void NewConnection();

    //关闭socket
    void closeSocket();

    void checkLink();

    void printMsg();


private:

    void sendMsg(const QString &msg);

    qint64 curTime;

    int port;
    QTimer *heartTimer;

    qint64 getLongUnixTime();

    //监听套接字
    QTcpServer *m_server;
    //通信套接字
    QTcpSocket *m_socket;

    QThreadPool pool;

};

#endif // TESTSERVER_H
