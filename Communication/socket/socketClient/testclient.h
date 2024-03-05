#ifndef TESTCLIENT_H
#define TESTCLIENT_H
#include <QTcpSocket> //通信套接字
#include <QTimer>
#include <QObject>
class TestClient: public QObject
{
    Q_OBJECT    //添加这个宏是为了实现信号和槽的通信
public:
    TestClient();

    //是否连接标志
    bool bislink;

    int gzmsgnum;
    int heartnum;
    int sendNum;

    int testid;
    int gbtNo;


    //启动
    //初始化连接
    void initClient();

    void test();





private slots:
    //关闭连接
    void slotCloseLink();

    //读取文字
    void slotReadyRead();

    //发送信号
    void sendheart();

    void testSend();

private:
    //配置项
    QString id;
    qint16 port;



    //定时器
    QTimer *heart_timer;
    QTimer *timer;

    QTcpSocket *m_Socket;  //通信套接字

    void sendMsg(const QString &msg);

};

#endif // TESTCLIENT_H
