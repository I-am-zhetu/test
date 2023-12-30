/********************************************************************************
** Form generated from reading UI file 'Serial.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERIAL_H
#define UI_SERIAL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SerialClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SerialClass)
    {
        if (SerialClass->objectName().isEmpty())
            SerialClass->setObjectName(QString::fromUtf8("SerialClass"));
        SerialClass->resize(600, 400);
        menuBar = new QMenuBar(SerialClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        SerialClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SerialClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        SerialClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(SerialClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        SerialClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(SerialClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        SerialClass->setStatusBar(statusBar);

        retranslateUi(SerialClass);

        QMetaObject::connectSlotsByName(SerialClass);
    } // setupUi

    void retranslateUi(QMainWindow *SerialClass)
    {
        SerialClass->setWindowTitle(QCoreApplication::translate("SerialClass", "Serial", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SerialClass: public Ui_SerialClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERIAL_H
