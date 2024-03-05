#include "testclient.h"
#include <QHostAddress>
#include <QSettings>
#include <QDebug>
#include <QThread>
TestClient::TestClient()
{
    //读取配置文件
    QSettings *configIni=new QSettings("./config.ini",QSettings::IniFormat);
    id = configIni->value("GZ/id").toString();
    port = configIni->value("GZ/port").toInt();
    delete configIni;//读取完成删除指针

    gzmsgnum=0;
    heartnum=0;
    sendNum = 0;

    testid=1000;
    gbtNo   =1;

    bislink=false;
}


void TestClient::initClient()
{
    m_Socket = NULL;//先为空

    //分配空间，指定父对象
    m_Socket = new QTcpSocket();
    m_Socket->abort();//   取消原有连接

    //连接服务器
    m_Socket->connectToHost(id,port);
    //    m_Socket->connectToHost(id,port);
    if (m_Socket->waitForConnected(30000))
    {
        qDebug()<<"link server success";
        bislink=true;

        //通过信号接收服务器数据
        connect(m_Socket, &QTcpSocket::readyRead,this, &TestClient::slotReadyRead);
        //断开连接
        connect(m_Socket, &QTcpSocket::disconnected,this, &TestClient::slotCloseLink);

        //        //启动定时器每1min去发送心跳
        //        heart_timer = new QTimer();
        //        connect(heart_timer, SIGNAL(timeout()),this, SLOT(sendheart()));
        //        heart_timer->start(60 * 1000);//1min


        //启动定时器每1min去发送心跳
        timer = new QTimer();
        connect(timer, SIGNAL(timeout()),this, SLOT(testSend()));
        timer->start(10 * 1000);//1min


    }else
    {
        qDebug()<<"link server failed";
        bislink=false;
        initClient();
    }
}

void TestClient::test()
{
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()),this, SLOT(testSend()));
    timer->start(3 * 1000);//1min



}

void TestClient::slotCloseLink()
{
    qDebug()<<"link server break";

    bislink = false;
    heart_timer->stop();

    //等待write数据发完，并发送disconnected()信号
    m_Socket->disconnectFromHost();
    if (m_Socket->state() == QAbstractSocket::UnconnectedState ||
            m_Socket->waitForDisconnected(1000))
        qDebug("Server has been Disconnected!");
}

void TestClient::slotReadyRead()
{
    //获取对方发送的内容
    QByteArray array = m_Socket->readAll();
    if(!array.isEmpty())
    {
        QString recvMsg=array;
        recvMsg.replace(" ","");//去掉空格
        //        qDebug()<<"recv msg:"<<recvMsg;
    }
}

void TestClient::sendheart()
{
    //连接上才发送心跳
    if(bislink == true)
    {
        QString heart="FE";
        m_Socket->write(heart.toLatin1());
        m_Socket->flush();
        heartnum += 1;
    }
    else if(heartnum == 60)//1h
    {
        qDebug()<<"not revc heart";
        //重连
        slotCloseLink();
        initClient();
    }
}



void TestClient::testSend()
{

    if(bislink)
    {
        sendMsg("FE");

        QString one="LBT,"+QString::number(testid)+","+QString::number(gbtNo)+",123123;";
        sendMsg(one);
//                QThread::sleep(1);
//                gbtNo+=1;
//                testid+=1;

//                sendMsg("LBT,"+QString::number(testid)+","+QString::number(gbtNo)+",456456;");
////                QThread::sleep(1);
//                gbtNo+=1;
//                testid+=1;
//                sendMsg("LBT,"+QString::number(testid)+","+QString::number(gbtNo)+",789789;");

        QThread::sleep(6);

        QString two="REST,1;";
        sendMsg(two);

        QString str3="XRFID,"+QString::number(testid)+",1592877500832;";
        sendMsg(str3);
        QThread::sleep(1);

        QString str4="ORFID,"+QString::number(testid)+",1, xjxjjkss.jpg ,1592877500832;";
        sendMsg(str4);

        QThread::sleep(1);

        QString str5="OPEN,"+QString::number(testid)+",1, 1592877500832;";
        sendMsg(str5);

        sendNum += 1;
//        if(sendNum == 200)
//        {
//            timer->stop();
//        }
        qDebug()<<"send num:"<<sendNum;

        gbtNo +=1;
        if(gbtNo == 4)
        {
            gbtNo =1 ;
        }

        testid += 1;
        if(testid == 1020)
        {
            testid =1000 ;
        }

    }


}

void TestClient::sendMsg(const QString &msg)
{
    m_Socket->write(msg.toLatin1());
    m_Socket->flush();
}




