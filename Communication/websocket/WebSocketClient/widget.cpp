#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    QString str("wss://localhost:8888");
    QString uid("aa");
    wsclient = new WSClient();
    wsclient->init(str,uid);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{

}

void Widget::on_pushButton_2_clicked()
{
    QString text = ui->textEdit_2->toPlainText();
    wsclient->sendMessage(text,true,"aa","bb");
    ui->textEdit_2->clear();
}
