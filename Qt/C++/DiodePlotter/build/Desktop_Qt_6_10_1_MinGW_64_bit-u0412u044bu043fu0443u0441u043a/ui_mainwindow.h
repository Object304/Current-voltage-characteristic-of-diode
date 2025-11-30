/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCharts/QChartView>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QChartView *chartView;
    QPushButton *buttonCom;
    QPushButton *buttonConv;
    QPushButton *buttonPlot;
    QPushButton *buttonClearLog;
    QLineEdit *portInput;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1000, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        chartView = new QChartView(centralwidget);
        chartView->setObjectName("chartView");
        chartView->setGeometry(QRect(180, 10, 801, 551));
        buttonCom = new QPushButton(centralwidget);
        buttonCom->setObjectName("buttonCom");
        buttonCom->setGeometry(QRect(10, 40, 151, 21));
        buttonConv = new QPushButton(centralwidget);
        buttonConv->setObjectName("buttonConv");
        buttonConv->setGeometry(QRect(10, 70, 151, 21));
        buttonPlot = new QPushButton(centralwidget);
        buttonPlot->setObjectName("buttonPlot");
        buttonPlot->setGeometry(QRect(10, 100, 151, 21));
        buttonClearLog = new QPushButton(centralwidget);
        buttonClearLog->setObjectName("buttonClearLog");
        buttonClearLog->setGeometry(QRect(10, 130, 151, 21));
        portInput = new QLineEdit(centralwidget);
        portInput->setObjectName("portInput");
        portInput->setGeometry(QRect(10, 10, 151, 20));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1000, 17));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        buttonCom->setText(QCoreApplication::translate("MainWindow", "Enter COM port number", nullptr));
        buttonConv->setText(QCoreApplication::translate("MainWindow", "Start conversion", nullptr));
        buttonPlot->setText(QCoreApplication::translate("MainWindow", "Plot", nullptr));
        buttonClearLog->setText(QCoreApplication::translate("MainWindow", "Clear log", nullptr));
        portInput->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
