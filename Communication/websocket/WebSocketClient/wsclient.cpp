#include "wsclient.h"
#include <QEventLoop>
#include <QFileInfo>
#include <QTimer>
#include <QDebug>


WSClient::WSClient()
{
    execurrentPath = QCoreApplication::applicationDirPath();
    execurrentPath.replace("\\","//");
    execurrentPath = execurrentPath + "//file//";
}

void WSClient::init(QString &url,QString &UserId)
{
    qDebug()<<url;

    currentID = UserId;

    connect(&m_webSocket, &QWebSocket::connected, this, &WSClient::onConnected);
    connect(&m_webSocket, QOverload<const QList<QSslError>&>::of(&QWebSocket::sslErrors),
            this, &WSClient::onSslErrors);

//    QSslConfiguration config = m_webSocket.sslConfiguration();
//    config.setPeerVerifyMode(QSslSocket::VerifyNone);
//    config.setProtocol(QSsl::TlsV1SslV3);
//    m_webSocket.setSslConfiguration(config);

    m_webSocket.open(QUrl(url));
}

void WSClient::onConnected()
{

    QString GoOnline ="GoOnlineStart|"+currentID+"|GoOnlineEnd";
    sendMessage(GoOnline,true,currentID,NULL);

    //接收文本
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &WSClient::dealText);
    //接收文件
    connect(&m_webSocket, &QWebSocket::binaryMessageReceived, this, &WSClient::dealFile);

    //    connect(&m_webSocket, &QWebSocket::textMessageReceived,this,[=](QString text)
    //    {

    //    });

    //    connect(&m_webSocket, &QWebSocket::binaryMessageReceived,this,[=](QByteArray array)
    //    {

    //    });
}

void WSClient::sendMessage(const QString &text, bool isText,QString sender,QString receiver)
{
    //判断是文本还是文件
    if(true == isText)
    {
        //发送文本
        m_webSocket.sendTextMessage(text);
    }else//文件
    {
        QStringList strList = text.split(".");
        QString fileFormat = strList[1];
        int m_blockSize = 64*1024;
        QFileInfo fileinfo(text);
        qint64 fileSize =  fileinfo.size();
        QString fileName = fileinfo.fileName();
        //数据流头部：
        QString head = QString("FILESTART|%1#%2").arg(fileName).arg(fileSize);

#ifdef QT_NO_DEBUG
#else
        qDebug()<<"file deal*********************************";
        qDebug()<<"fileFormat:"<<fileFormat;
        qDebug()<<"fileName:"<<fileName;
        qDebug()<<"fileSize:"<<fileSize;
        qDebug()<<"head:"<<head;
#endif


        QFile file(text);
        file.open(QIODevice::ReadOnly);
        qint64 sendSize = 0;
        qint64 writesize = fileSize;

        //第一步：先发送头部信息
        m_webSocket.sendBinaryMessage(head.toUtf8());

        qint64 len = 0;
        QByteArray array = nullptr;
        do{

            //每次读64k数据
            //array = file.read(qMin(writesize,m_blockSize));
            array = file.read(m_blockSize);
            //第二步：发送文件内容
            len = m_webSocket.sendBinaryMessage(array);
            sendSize += len;
            writesize -= len;
#ifdef QT_NO_DEBUG
#else
            qDebug() << QStringLiteral("sendSize:")<<sendSize;
#endif
            //由于发送太快，增加延时防止缓冲区来不及清理造成崩溃
            if (sendSize%(100*1024*1024) == 0)
            {
                QEventLoop eventloop;
                //3000表示3000ms，即3秒，可根据实际情况修改，其他的不用变
                QTimer::singleShot(1000, &eventloop, SLOT(quit()));
                eventloop.exec();
            }
        }while(writesize>0);
        if(sendSize == fileSize)//检验文件信息
        {
            //第三步：发送文件结束符
            QString endStr = sender+"|"+receiver+"|"+fileName+"|FILEEND";
            m_webSocket.sendBinaryMessage(endStr.toUtf8());
#ifdef QT_NO_DEBUG
#else
            qDebug() << QStringLiteral("FILE send end");
#endif
        }
        file.close();
    }
}

void WSClient::onSslErrors(const QList<QSslError> &errors)
{
    Q_UNUSED(errors);//ignore warning;
    m_webSocket.ignoreSslErrors();
}

void WSClient::dealText(QString text)
{
    //limit max size
#ifdef QT_NO_DEBUG
#else
    qDebug() << "text:" << text;
#endif
    emit showTextSignals(text);//接收文本后显示到窗口
}

void WSClient::dealFile(QByteArray array)
{
    QString fileName;
    //接收文件头部
    if(array.left(10) == "FILESTART|")
    {
        isFile = true;
        QString str = array;
        str = str.split("|").last();
        fileName = str.section("#", 0, 0);
        m_fileSize = str.section("#", 1, 1).toDouble();
        m_recvSize = 0;

        //保存到指定的路径
        m_file.setFileName(execurrentPath+fileName);
        //打开文件
        bool isOK = m_file.open(QIODevice::WriteOnly);
        if(!isOK)
        {
#ifdef QT_NO_DEBUG
#else
            qDebug() << QStringLiteral("open file failed");
#endif
        }
    }
    //接收文件尾部
    //sender|receiver|filename|FILEEND
    else if(array.right(8) == "|FILEEND")
    {
        m_file.close();
#ifdef QT_NO_DEBUG
#else
        qDebug() << QStringLiteral("FILE recv end");
#endif
        QString fileEndStr = array;
        qDebug()<<fileEndStr;

        emit showFileSignals(fileEndStr);//接收文本后显示到窗口
    }
    else
    {
        //接收文件的二进制数据
        if (true == isFile)
        {
            qint64 len = m_file.write(array);
            m_recvSize += len;
#ifdef QT_NO_DEBUG
#else
            qDebug()  <<QStringLiteral("recv file ok,m_recvSize:")<< m_recvSize;
#endif

        }
    }
}
