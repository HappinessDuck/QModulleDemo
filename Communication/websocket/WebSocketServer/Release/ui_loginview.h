/********************************************************************************
** Form generated from reading UI file 'loginview.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINVIEW_H
#define UI_LOGINVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginView
{
public:
    QPushButton *pushButton;
    QLabel *label;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QLabel *label_3;
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;

    void setupUi(QDialog *LoginView)
    {
        if (LoginView->objectName().isEmpty())
            LoginView->setObjectName(QStringLiteral("LoginView"));
        LoginView->resize(663, 359);
        pushButton = new QPushButton(LoginView);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(410, 190, 91, 51));
        label = new QLabel(LoginView);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(220, 30, 191, 81));
        QFont font;
        font.setFamily(QStringLiteral("Agency FB"));
        font.setPointSize(28);
        label->setFont(font);
        widget = new QWidget(LoginView);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(150, 190, 229, 53));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_2 = new QLabel(widget);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout_2->addWidget(label_2);

        label_3 = new QLabel(widget);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout_2->addWidget(label_3);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        lineEdit = new QLineEdit(widget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        verticalLayout->addWidget(lineEdit);

        lineEdit_2 = new QLineEdit(widget);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));

        verticalLayout->addWidget(lineEdit_2);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(LoginView);

        QMetaObject::connectSlotsByName(LoginView);
    } // setupUi

    void retranslateUi(QDialog *LoginView)
    {
        LoginView->setWindowTitle(QApplication::translate("LoginView", "Dialog", nullptr));
        pushButton->setText(QApplication::translate("LoginView", "\347\231\273\345\275\225", nullptr));
        label->setText(QApplication::translate("LoginView", "PC\345\256\242\346\210\267\347\253\257", nullptr));
        label_2->setText(QApplication::translate("LoginView", "\350\264\246\345\217\267:", nullptr));
        label_3->setText(QApplication::translate("LoginView", "\345\257\206\347\240\201\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginView: public Ui_LoginView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINVIEW_H
