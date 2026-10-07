/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGraphicsView *graphicsView;
    QLabel *labelState;
    QLabel *labelBattery;
    QPushButton *btnLostLink;
    QPushButton *btnLowBattery;
    QPushButton *btnRestore;
    QPushButton *btnReset;
    QTextEdit *textLog;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(900, 550);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName("graphicsView");
        graphicsView->setGeometry(QRect(0, 0, 641, 501));
        labelState = new QLabel(centralwidget);
        labelState->setObjectName("labelState");
        labelState->setGeometry(QRect(660, 50, 151, 20));
        labelBattery = new QLabel(centralwidget);
        labelBattery->setObjectName("labelBattery");
        labelBattery->setGeometry(QRect(660, 70, 111, 16));
        btnLostLink = new QPushButton(centralwidget);
        btnLostLink->setObjectName("btnLostLink");
        btnLostLink->setGeometry(QRect(660, 140, 111, 32));
        btnLowBattery = new QPushButton(centralwidget);
        btnLowBattery->setObjectName("btnLowBattery");
        btnLowBattery->setGeometry(QRect(660, 170, 111, 32));
        btnRestore = new QPushButton(centralwidget);
        btnRestore->setObjectName("btnRestore");
        btnRestore->setGeometry(QRect(660, 200, 111, 32));
        btnReset = new QPushButton(centralwidget);
        btnReset->setObjectName("btnReset");
        btnReset->setGeometry(QRect(660, 230, 111, 32));
        textLog = new QTextEdit(centralwidget);
        textLog->setObjectName("textLog");
        textLog->setGeometry(QRect(660, 290, 171, 121));
        textLog->setReadOnly(true);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 900, 22));
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
        labelState->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\321\201\321\202\320\276\321\217\320\275\320\270\320\265: \320\237\320\236\320\233\320\201\320\242", nullptr));
        labelBattery->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\321\200\321\217\320\264: 80%", nullptr));
        btnLostLink->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\321\202\320\265\321\200\321\217 \321\201\320\262\321\217\320\267\320\270", nullptr));
        btnLowBattery->setText(QCoreApplication::translate("MainWindow", "\320\240\320\260\320\267\321\200\321\217\320\264 \320\261\320\260\321\202\320\260\321\200\320\265\320\270", nullptr));
        btnRestore->setText(QCoreApplication::translate("MainWindow", "\320\222\320\276\321\201\321\201\321\202\320\260\320\275\320\276\320\262\320\270\321\202\321\214", nullptr));
        btnReset->setText(QCoreApplication::translate("MainWindow", "\320\241\320\261\321\200\320\276\321\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
