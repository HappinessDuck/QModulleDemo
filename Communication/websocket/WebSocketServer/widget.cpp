#include "widget.h"
#include "ui_widget.h"
#include <QHostAddress>
#include <QDebug>
#include <QSettings>
#include <QListWidgetItem>

#include <QFileInfo>
#include <QMessageBox>

#include <QSslCertificate>
#include <QSslKey>

//#include <QClipboard> //剪切板
//#include <QApplication>
#include <QEventLoop>
#include <QTimer>




Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QString execurrentPath = QCoreApplication::applicationDirPath();
    QString configPath =execurrentPath+"/config.ini";
    //    configPath = configPath.replace("/","\\");

#ifdef QT_NO_DEBUG
#else
    qDebug()<<"configPath:"<<configPath;
#endif
    QSettings *config = new QSettings(configPath, QSettings::IniFormat);

    QString title;
    title = config->value("server/title").toString();
    setWindowTitle(title);
    port = config->value("server/port").toInt();
    delete config;

#ifdef QT_NO_DEBUG
#else
    qDebug()<<"title:"<<title;
    qDebug()<<"port:"<<port;
#endif
    //QWebSocketServer::SslMode 运行模式：安全模式（SecureMode）wss，非安全模式（NonSecureMode）ws

    //ws模式
    ws_server=new QWebSocketServer(QStringLiteral("testServer"),QWebSocketServer::NonSecureMode,this);

    //wws模式
    //    ws_server=new QWebSocketServer(QStringLiteral("testServer"),QWebSocketServer::SecureMode,this);
    //    QSslConfiguration sslConfiguration;
    //    QString certPath = execurrentPath+"/myserverca.cert";
    //    QString keyPath = execurrentPath+"/myserver.key";
    //    QFile certFile(certPath);
    //    QFile keyFile(keyPath);
    //    certFile.open(QIODevice::ReadOnly);
    //    keyFile.open(QIODevice::ReadOnly);
    //    QSslCertificate certificate(&certFile, QSsl::Pem);
    //    QSslKey sslKey(&keyFile, QSsl::Rsa, QSsl::Pem);
    //    certFile.close();
    //    keyFile.close();
    //    sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyNone);
    //    sslConfiguration.setLocalCertificate(certificate);
    //    sslConfiguration.setPrivateKey(sslKey);
    //    sslConfiguration.setProtocol(QSsl::TlsV1SslV3);
    //    ws_server->setSslConfiguration(sslConfiguration);


    if(ws_server->listen(QHostAddress::Any,port))
    {
        //新的连接进来
        connect(ws_server,&QWebSocketServer::newConnection,this,&Widget::getNewConnection);
    }



    isFile = false;
    LinkSocketNum = 0;
}

Widget::~Widget()
{
    delete ui;
}



void Widget::getNewConnection()
{
    if(ws_server->hasPendingConnections()) //有未处理的连接
    {
        QWebSocket * webSocket = ws_server->nextPendingConnection();
        //把socket加进队列
        clientList << webSocket;

        LinkSocketNum += 1;

        qDebug()<<"LinkSocketNum:"<<LinkSocketNum;
        qDebug()<<"webSocket:"<<webSocket;
        qDebug()<<"origin:"<<webSocket->origin();

        //服务器增加连接用户
        //把连接的用户添加到map
        QString UserName;
        UserName = webSocket->origin();


        //添加连接对象到列表
        QListWidgetItem * item = new QListWidgetItem;
        item->setText(UserName);
        ui->listWidget->addItem(item);

        UserMap.insert(UserName,webSocket->origin());
        qDebug()<<"UserMap:"<<UserMap.value(UserName);

        ui->textEdit->append(UserName + "连接上了");


#ifdef QT_NO_DEBUG
#else
        qDebug()<<UserName<<"连接上了";
#endif


    //断开连接
    connect(webSocket,&QWebSocket::disconnected,this,[webSocket,this]
    {
        LinkSocketNum -= 1;
        clientList.removeOne(webSocket);
        for (int j = 0; j < ui->listWidget->count(); j++)
        {
            QListWidgetItem *item = ui->listWidget->item(j);

            //遍历查看谁退出了
            if (UserMap.value(item->text()) == webSocket->origin())
            {
#ifdef QT_NO_DEBUG
#else
                qDebug()<<item->text()<<"断开连接了";
#endif
                ui->textEdit->append(item->text() + "断开连接了");
                ui->listWidget->removeItemWidget(item);
                delete item;
                break;
            }
        }
         webSocket->deleteLater();

    });


    //服务器接收字符
    connect(webSocket, &QWebSocket::textMessageReceived,this,[=](QString message)
    {
        qDebug()<<"recv:"<<message;
        ui->textEdit->append(sendUserName + "：");
        ui->textEdit->append(message);

//        if(message.contains("TEXTEND|"))
//        {
//#ifdef QT_NO_DEBUG
//#else
//            qDebug()<<"recv:"<<message;
//#endif
//            sendUserName = message.split("|").at(1);
//            recvUserName = message.split("|").last();
//            //字符串转发
//            transferOrSave(message,true,recvUserName);

//            ui->textEdit->append(sendUserName + "：");
//            ui->textEdit->append(message.split("|").at(2));
//        }



    });

    connect(webSocket, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(onError(QAbstractSocket::SocketError)));//这里参数是枚举 用qt5的方式这里编译不过

    //接收二进制文件
    connect(webSocket,&QWebSocket::binaryMessageReceived,this,[=](QByteArray array)
    {
        //接收文件头部
        if (array.contains("FILESTART!"))
        {
            isFile = true;
            QString str = array;
            str = str.split("|").last();
            QString fileName = str.section("#", 0, 0);
            m_fileSize = str.section("#", 1, 1).toDouble();
            m_recvSize = 0;

            m_file.setFileName(fileName);
            //打开文件
            bool isOK = m_file.open(QIODevice::WriteOnly);
            if(!isOK)
            {
                qDebug() << QStringLiteral("打开文件失败");
#ifdef QT_NO_DEBUG
#else
                qDebug() << QStringLiteral("打开文件失败");
#endif
            }
        }
        //接收文件尾部
        else if (array == "FILEEND!")
        {
            QString user = array;
            user = user.split("|").last();

            ui->textEdit->append("接收文件成功");
#ifdef QT_NO_DEBUG
#else
            qDebug() << QStringLiteral("接收文件成功,m_recvSize:")<< m_recvSize;
            qDebug() << QStringLiteral("接收文件成功,实际:")<< m_fileSize;
#endif
            m_file.close();
        }
        else
        {
            //接收文件的二进制数据
            if (true == isFile)
            {
                qint64 len = m_file.write(array);
                m_recvSize += len;
                qDebug()  << QStringLiteral("接收") <<m_recvSize;
#ifdef QT_NO_DEBUG
#else
                qDebug()  << QStringLiteral("接收") <<m_recvSize;
#endif
            }
        }
    });

}


}

//根据他要发送的对象，判断是否在线直接发送或者保存在服务器本地
bool Widget::transferOrSave(const QString &text,bool isText,QString userName)
{

    //判断是否在线
    //在线--直接转发
    sendMessage(text,isText,userName);

    //不在线--保存本地，等待上线后再次发送
    //new 任务队列
    return true;
}


void Widget::sendMessage(const QString &text,bool isText,QString recvUserName)
{
#ifdef QT_NO_DEBUG
#else
    qDebug()<<"clientList "<<clientList.count();
    qDebug()<<"send to "<<recvUserName;
    qDebug()<<"count: "<<UserMap.count();
    qDebug()<<"send to "<<UserMap.value(recvUserName);
#endif

    //在连接列表查询待发送的对象
    QWebSocket * webSocket = nullptr;
    for (auto socket:clientList)
    {
        if(socket->origin() == UserMap.value(recvUserName))
        {
            webSocket = socket;
#ifdef QT_NO_DEBUG
#else
            qDebug()<<"find this websocket";
#endif
            break;
        }
    }

    if(webSocket)
    {
        QString head;
        //判断是文本还是文件
        if(true == isText)
        {
            //发送文本
            webSocket->sendTextMessage(text);

        }else//文件
        {
            QStringList strList = text.split(".");
            QString fileFormat = strList[1];
            int m_blockSize = 64*1024;
            QFileInfo fileinfo(text);
            qint64 fileSize =  fileinfo.size();
            QString fileName = fileinfo.fileName();
            //数据流头部：
            head = QString("FILESTART!|%1#%2").arg(fileName).arg(fileSize);

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
            webSocket->sendBinaryMessage(head.toUtf8());

            qint64 len = 0;
            QByteArray array = nullptr;
            do{

                //每次读64k数据
                //                array = file.read(qMin(writesize,m_blockSize));
                array = file.read(m_blockSize);
                //第二步：发送文件内容
                len = webSocket->sendBinaryMessage(array);
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
#ifdef QT_NO_DEBUG
#else
                qDebug() << QStringLiteral("文件发送完毕");
#endif
                //第三步：发送文件结束符
                webSocket->sendBinaryMessage("FILEEND!");
                ui->textEdit->append("文件发送成功");
            }
            file.close();

        }

        ui->textEdit->append("服务端给" + recvUserName + "发送:");
        ui->textEdit->append(text);

    }
}

void Widget::onError(QAbstractSocket::SocketError error)
{
    QWebSocket * webSocket = qobject_cast<QWebSocket *>(sender());
    //    ui->textEdit->append(webSocket->origin() + "出错" + webSocket->errorString());

#ifdef QT_NO_DEBUG
#else
    qDebug() << webSocket->errorString();
#endif
}


bool Widget::isFileExist(QString fullFileName)
{
    QFileInfo fileInfo(fullFileName);
    if(fileInfo.isFile())
    {
        return true;
    }
    return false;
}



void Widget::on_pushButton_clicked()
{
    QString msg = ui->textEdit_2->toPlainText();
    ui->textEdit_2->clear();
#ifdef QT_NO_DEBUG
#else
    qDebug()<<"msg:"<<msg;
#endif
    if(msg.isEmpty())
    {
        QMessageBox::warning(this, "send","发送内容不可为空");
        return;
    }

    bool isText = true;

    //判断是否是文件，是否存在本地目录
    if(isFileExist(msg.right(msg.length()-8)))
    {
        isText = false;
        //"file:///E:/Desktop/ic_launcher.png"
        msg = msg.right(msg.length()-8);
        //是文件
#ifdef QT_NO_DEBUG
#else
        qDebug()<<"send file exist";
#endif
    }else
    {
        sendMessage(msg,isText,"");
    }


}
