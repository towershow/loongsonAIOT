/********************************************************************************
** Form generated from reading UI file 'can.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAN_H
#define UI_CAN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CAN
{
public:
    QWidget *centralwidget;
    QListView *listView;
    QLabel *label;
    QToolButton *toolButton_mainWindow;
    QToolButton *toolButton_Oscilloscope;
    QToolButton *toolButton_CAN;
    QToolButton *toolButton_Record;
    QToolButton *toolButton_deviceStatus;
    QLabel *label_2;

    void setupUi(QMainWindow *CAN)
    {
        if (CAN->objectName().isEmpty())
            CAN->setObjectName(QString::fromUtf8("CAN"));
        CAN->resize(1920, 1017);
        centralwidget = new QWidget(CAN);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        listView = new QListView(centralwidget);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->setGeometry(QRect(0, 0, 1920, 1017));
        listView->setStyleSheet(QString::fromUtf8("background-image: url(:/images/can_ui/background.png);"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(60, 0, 441, 165));
        label->setStyleSheet(QString::fromUtf8("image: url(:/images/can_ui/Gowin_logo.png);"));
        toolButton_mainWindow = new QToolButton(centralwidget);
        toolButton_mainWindow->setObjectName(QString::fromUtf8("toolButton_mainWindow"));
        toolButton_mainWindow->setGeometry(QRect(560, 20, 251, 101));
        toolButton_Oscilloscope = new QToolButton(centralwidget);
        toolButton_Oscilloscope->setObjectName(QString::fromUtf8("toolButton_Oscilloscope"));
        toolButton_Oscilloscope->setGeometry(QRect(810, 20, 251, 101));
        toolButton_CAN = new QToolButton(centralwidget);
        toolButton_CAN->setObjectName(QString::fromUtf8("toolButton_CAN"));
        toolButton_CAN->setGeometry(QRect(1060, 20, 251, 101));
        toolButton_Record = new QToolButton(centralwidget);
        toolButton_Record->setObjectName(QString::fromUtf8("toolButton_Record"));
        toolButton_Record->setGeometry(QRect(1310, 20, 251, 101));
        toolButton_deviceStatus = new QToolButton(centralwidget);
        toolButton_deviceStatus->setObjectName(QString::fromUtf8("toolButton_deviceStatus"));
        toolButton_deviceStatus->setGeometry(QRect(1560, 20, 251, 101));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(1070, 100, 231, 51));
        label_2->setStyleSheet(QString::fromUtf8("image: url(:/images/can_ui/button_bottom.png);"));
        CAN->setCentralWidget(centralwidget);

        retranslateUi(CAN);

        QMetaObject::connectSlotsByName(CAN);
    } // setupUi

    void retranslateUi(QMainWindow *CAN)
    {
        CAN->setWindowTitle(QCoreApplication::translate("CAN", "MainWindow", nullptr));
        label->setText(QString());
        toolButton_mainWindow->setText(QString());
        toolButton_Oscilloscope->setText(QString());
        toolButton_CAN->setText(QString());
        toolButton_Record->setText(QString());
        toolButton_deviceStatus->setText(QString());
        label_2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class CAN: public Ui_CAN {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAN_H
