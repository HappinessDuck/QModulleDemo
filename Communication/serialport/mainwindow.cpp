#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    //    {
    //        QSerialPort serial;
    //        serial.setPort(info);
    //        if(serial.open(QIODevice::ReadWrite))
    //        {
    //            qDebug()<<serial.portName();
    //            serial.close();
    //        }
    //    }


}

MainWindow::~MainWindow()
{
    if(serial->isOpen())//如果串口已经打开了 先给他关闭了
    {
        serial->close();
    }
    delete serial;

    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    //    if(serial->isOpen())//如果串口已经打开了 先给他关闭了
    //    {
    //        serial->clear();
    //        serial->close();
    //    }

    serial = new QSerialPort;
    //1、设置串口名（如COM1）
    serial->setPortName("COM1");
    //2、打开串口
    serial->open(QIODevice::ReadWrite);
    //3、设置波特率（如115200）
    serial->setBaudRate(QSerialPort::Baud115200);//设置波特率为115200
    //4、设置数据位（如8）
    serial->setDataBits(QSerialPort::Data8);//设置数据位8
    //5、设置校验位（如0）
    serial->setParity(QSerialPort::NoParity); //校验位设置为0
    //6、设置停止位（如1）
    serial->setStopBits(QSerialPort::OneStop);//停止位设置为1
    //7、设置流控制
    serial->setFlowControl(QSerialPort::NoFlowControl);//设置为无流控制

    //连接信号槽
    QObject::connect(serial,&QSerialPort::readyRead,this,&MainWindow::ReadData);
//    qDebug()<<"open success";
    ui->textBrowser->append("open success");


}

void MainWindow::ReadData()
{
    QByteArray buf = serial->readAll();;
    if(!buf.isEmpty())
    {
        QByteArray aa = QByteArray::fromHex(buf);
        QString msg=aa.data();
        msg.replace(" ","");
        ui->textBrowser->append(msg);
//        qDebug()<<buf;
    }
}

void MainWindow::on_pushButton_2_clicked()
{
//    qDebug()<<"close serial";
    ui->textBrowser->append("close serial");
    //关闭串口
    serial->clear();
    serial->close();
    serial->deleteLater();
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->textBrowser->clear();
}
