//# **********************************************************
//# * Author        : wqj
//# * Email         : 1107097641@qq.com
//# * Create time   : 2023-11-04
//# * Filename      : main.py
//# * Description   : 加载html窗口并执行js函数--QWebEngineView只支持msvc编译，不支持mingw
//# * Copyright     : (c) 2023
//# **********************************************************
#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QVBoxLayout>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->resize(1280, 720);

    // 创建一个垂直布局
    QVBoxLayout* layout = new QVBoxLayout(this);

    view = new QWebEngineView(this);
    qDebug() << view->page()->profile();

    // 加载本地路径的HTML文件
    QString filePath = "E:/dist/index.html";
    QUrl url = QUrl::fromLocalFile(filePath);
    view->load(url);
	
	view.setUrl(QUrl(filePath));

    layout->addWidget(ui->pushButton);
    layout->addWidget(ui->pushButton_2);
    // 将QWebEngineView添加到布局中
    layout->addWidget(view);


    view->show();

}

Widget::~Widget()
{
    delete ui;
}

// 执行js代码
void Widget::run_js(QString data)
{
    view->page()->runJavaScript(data);
}

void Widget::on_pushButton_clicked()
{
    run_js("toHotList();");
}

void Widget::on_pushButton_2_clicked()
{
    run_js("goBack_Home();");
}
