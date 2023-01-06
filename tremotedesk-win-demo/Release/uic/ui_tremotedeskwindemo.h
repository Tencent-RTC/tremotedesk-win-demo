/********************************************************************************
** Form generated from reading UI file 'tremotedeskwindemo.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TREMOTEDESKWINDEMO_H
#define UI_TREMOTEDESKWINDEMO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_tremotedeskwindemoClass
{
public:
    QLineEdit *lineEdit_appkey;
    QLineEdit *lineEdit_roomId;
    QLineEdit *lineEdit_userId;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *login_;
    QPushButton *leave_;
    QTextEdit *log_;

    void setupUi(QWidget *tremotedeskwindemoClass)
    {
        if (tremotedeskwindemoClass->objectName().isEmpty())
            tremotedeskwindemoClass->setObjectName(QString::fromUtf8("tremotedeskwindemoClass"));
        tremotedeskwindemoClass->resize(1863, 714);
        lineEdit_appkey = new QLineEdit(tremotedeskwindemoClass);
        lineEdit_appkey->setObjectName(QString::fromUtf8("lineEdit_appkey"));
        lineEdit_appkey->setGeometry(QRect(150, 40, 241, 41));
        lineEdit_roomId = new QLineEdit(tremotedeskwindemoClass);
        lineEdit_roomId->setObjectName(QString::fromUtf8("lineEdit_roomId"));
        lineEdit_roomId->setGeometry(QRect(150, 100, 241, 41));
        lineEdit_userId = new QLineEdit(tremotedeskwindemoClass);
        lineEdit_userId->setObjectName(QString::fromUtf8("lineEdit_userId"));
        lineEdit_userId->setGeometry(QRect(150, 160, 241, 41));
        label = new QLabel(tremotedeskwindemoClass);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 40, 121, 41));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_2 = new QLabel(tremotedeskwindemoClass);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 100, 121, 41));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_3 = new QLabel(tremotedeskwindemoClass);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 160, 111, 41));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        login_ = new QPushButton(tremotedeskwindemoClass);
        login_->setObjectName(QString::fromUtf8("login_"));
        login_->setGeometry(QRect(150, 220, 151, 61));
        leave_ = new QPushButton(tremotedeskwindemoClass);
        leave_->setObjectName(QString::fromUtf8("leave_"));
        leave_->setGeometry(QRect(150, 300, 151, 61));
        log_ = new QTextEdit(tremotedeskwindemoClass);
        log_->setObjectName(QString::fromUtf8("log_"));
        log_->setGeometry(QRect(400, 40, 1451, 691));

        retranslateUi(tremotedeskwindemoClass);

        QMetaObject::connectSlotsByName(tremotedeskwindemoClass);
    } // setupUi

    void retranslateUi(QWidget *tremotedeskwindemoClass)
    {
        tremotedeskwindemoClass->setWindowTitle(QCoreApplication::translate("tremotedeskwindemoClass", "tremotedeskwindemo", nullptr));
        label->setText(QCoreApplication::translate("tremotedeskwindemoClass", "appKey:", nullptr));
        label_2->setText(QCoreApplication::translate("tremotedeskwindemoClass", "roomId:", nullptr));
        label_3->setText(QCoreApplication::translate("tremotedeskwindemoClass", "userId:", nullptr));
        login_->setText(QCoreApplication::translate("tremotedeskwindemoClass", "Login", nullptr));
        leave_->setText(QCoreApplication::translate("tremotedeskwindemoClass", "leave", nullptr));
    } // retranslateUi

};

namespace Ui {
    class tremotedeskwindemoClass: public Ui_tremotedeskwindemoClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TREMOTEDESKWINDEMO_H
