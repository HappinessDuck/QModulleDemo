#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QWebEngineView>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    QWebEngineView *view;
public slots:
    void run_js(QString data);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Widget *ui;


};

#endif // WIDGET_H
