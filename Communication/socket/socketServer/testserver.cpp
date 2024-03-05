#include "testserver.h"
#include <QSettings>
#include <QDateTime>
#include <QtConcurrent>
#include <QFuture>

TestServer::TestServer()
{
    QSettings *config = new QSettings("./config.ini", QSettings::IniFormat);
    port = config->value("GZ/port").toInt();
    delete config;
}

void TestServer::initServer()
{

    pool.setMaxThreadCount(3);

    m_server=NULL;
    m_socket=NULL;
    m_server = new QTcpServer();
    //将套接字设置为监听模式
    if(!m_server->listen(QHostAddress::Any,port))
    {
        qDebug()<<"wait listening error:"<<m_server;
    }else
    {
        qDebug()<<"wait listening ok:"<<m_server;
    }
    //通过信号接收客户端请求
    connect(m_server, &QTcpServer::newConnection,this, &TestServer::NewConnection);


}

void TestServer::ReadData()
{
    //读取缓冲区数据
    QByteArray buffer= m_socket->readAll();
    if(!buffer.isEmpty())
    {
        QString recvMsg=buffer;
        recvMsg.replace(" ","");
        qDebug()<<"recv msg: "<<recvMsg;
        sendMsg(recvMsg);
    }
}

void TestServer::NewConnection()
{
    //处理客户端的连接请求
    m_socket=m_server->nextPendingConnection();
    qDebug()<<"link Client success"<<m_socket;
    sendMsg("success");

    //连接信号, 接收客户端数据
    connect(m_socket, &QTcpSocket::readyRead,this, &TestServer::ReadData);
    //断开连接
    connect(m_socket, &QTcpSocket::disconnected,this, &TestServer::closeSocket);
}

void TestServer::closeSocket()
{
    qDebug() << "link Client break";
    //释放socket
    m_socket->close();
}

void TestServer::checkLink()
{
    //每5min检查一次心跳，如果超过有效期1min，则判客户端断开
    if (getLongUnixTime() - curTime > 2 * 60 * 1000)
    {
        qDebug()<<getLongUnixTime();
        heartTimer->stop();
        closeSocket();
    }
}

void TestServer::printMsg()
{
    qDebug() << "thread id " << QThread::currentThread() ;
    QThread::sleep(10);
    qDebug()<<"end";
}

void TestServer::sendMsg(const QString &msg)
{
    m_socket->write(msg.toLatin1());
    m_socket->flush();
}

qint64 TestServer::getLongUnixTime()
{
    QString timestamp = QString::number(QDateTime::currentMSecsSinceEpoch());
    qint64 time = timestamp.toLongLong();
    return time;
}

