/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QListView *listView;
    QLabel *label_logo;
    QLabel *label_background;
    QLabel *label_tree;
    QLabel *label_title;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_currentTime;
    QLabel *label_date;
    QLabel *label_teamname;
    QLabel *label_teamaccount;
    QToolButton *toolButton_mainWindow;
    QToolButton *toolButton_Oscilloscope;
    QToolButton *toolButton_CAN;
    QToolButton *toolButton_Record;
    QToolButton *toolButton_deviceStatus;
    QLabel *label_3;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1920, 1017);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        listView = new QListView(centralwidget);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->setGeometry(QRect(0, 0, 1920, 1017));
        listView->setStyleSheet(QString::fromUtf8("background-image: url(:/images/mainwindow_ui/background.png);"));
        label_logo = new QLabel(centralwidget);
        label_logo->setObjectName(QString::fromUtf8("label_logo"));
        label_logo->setGeometry(QRect(60, 0, 441, 165));
        label_logo->setStyleSheet(QString::fromUtf8("image: url(:/images/mainwindow_ui/Gowin_logo.png);"));
        label_background = new QLabel(centralwidget);
        label_background->setObjectName(QString::fromUtf8("label_background"));
        label_background->setGeometry(QRect(10, 160, 1881, 821));
        label_background->setStyleSheet(QString::fromUtf8("image: url(:/images/mainwindow_ui/screen.png);"));
        label_background->setAlignment(Qt::AlignCenter);
        label_tree = new QLabel(centralwidget);
        label_tree->setObjectName(QString::fromUtf8("label_tree"));
        label_tree->setGeometry(QRect(190, 190, 91, 131));
        label_tree->setStyleSheet(QString::fromUtf8("image: url(:/images/mainwindow_ui/label_tree.png);"));
        label_title = new QLabel(centralwidget);
        label_title->setObjectName(QString::fromUtf8("label_title"));
        label_title->setGeometry(QRect(350, 230, 271, 71));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(310, 220, 1421, 81));
        QFont font;
        font.setPointSize(30);
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(240, 440, 1401, 161));
        label_2->setStyleSheet(QString::fromUtf8("image: url(:/images/mainwindow_ui/Title.png);"));
        label_currentTime = new QLabel(centralwidget);
        label_currentTime->setObjectName(QString::fromUtf8("label_currentTime"));
        label_currentTime->setGeometry(QRect(880, 350, 301, 61));
        QFont font1;
        font1.setPointSize(26);
        label_currentTime->setFont(font1);
        label_currentTime->setLayoutDirection(Qt::LeftToRight);
        label_currentTime->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_currentTime->setScaledContents(false);
        label_date = new QLabel(centralwidget);
        label_date->setObjectName(QString::fromUtf8("label_date"));
        label_date->setGeometry(QRect(870, 410, 341, 51));
        label_date->setFont(font1);
        label_date->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        label_teamname = new QLabel(centralwidget);
        label_teamname->setObjectName(QString::fromUtf8("label_teamname"));
        label_teamname->setGeometry(QRect(1250, 770, 441, 111));
        QFont font2;
        font2.setPointSize(20);
        label_teamname->setFont(font2);
        label_teamname->setStyleSheet(QString::fromUtf8("image: url(:/images/mainwindow_ui/Team_name.png);\n"
"color: rgb(255, 255, 255);"));
        label_teamname->setAlignment(Qt::AlignCenter);
        label_teamaccount = new QLabel(centralwidget);
        label_teamaccount->setObjectName(QString::fromUtf8("label_teamaccount"));
        label_teamaccount->setGeometry(QRect(1120, 860, 571, 61));
        label_teamaccount->setFont(font2);
        label_teamaccount->setStyleSheet(QString::fromUtf8("image: url(:/images/mainwindow_ui/Team_account.png);\n"
"color: rgb(255, 255, 255);"));
        label_teamaccount->setTextFormat(Qt::RichText);
        label_teamaccount->setAlignment(Qt::AlignCenter);
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
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(570, 90, 231, 71));
        label_3->setStyleSheet(QString::fromUtf8("image: url(:/images/mainwindow_ui/button_bottom.png);"));
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label_logo->setText(QString());
        label_background->setText(QString());
        label_tree->setText(QString());
        label_title->setText(QString());
        label->setText(QCoreApplication::translate("MainWindow", "2024\345\271\264\345\205\250\345\233\275\345\244\247\345\255\246\347\224\237\345\265\214\345\205\245\345\274\217\350\212\257\347\211\207\345\222\214\344\270\216\347\263\273\347\273\237\350\256\276\350\256\241\347\253\236\350\265\233\342\200\224\342\200\224FPGA\345\210\233\346\226\260\350\256\276\350\256\241\350\265\233\351\201\223", nullptr));
        label_2->setText(QString());
        label_currentTime->setText(QString());
        label_date->setText(QString());
        label_teamname->setText(QString());
        label_teamaccount->setText(QString());
        toolButton_mainWindow->setText(QString());
        toolButton_Oscilloscope->setText(QString());
        toolButton_CAN->setText(QString());
        toolButton_Record->setText(QString());
        toolButton_deviceStatus->setText(QString());
        label_3->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
