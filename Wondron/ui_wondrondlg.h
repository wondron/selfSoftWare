/********************************************************************************
** Form generated from reading UI file 'wondrondlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WONDRONDLG_H
#define UI_WONDRONDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_wondronDlg
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *layout;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *wondronDlg)
    {
        if (wondronDlg->objectName().isEmpty())
            wondronDlg->setObjectName(QString::fromUtf8("wondronDlg"));
        wondronDlg->resize(800, 600);
        centralwidget = new QWidget(wondronDlg);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        layout = new QVBoxLayout();
        layout->setObjectName(QString::fromUtf8("layout"));

        verticalLayout_2->addLayout(layout);


        horizontalLayout->addWidget(groupBox);

        wondronDlg->setCentralWidget(centralwidget);
        menubar = new QMenuBar(wondronDlg);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 23));
        wondronDlg->setMenuBar(menubar);
        statusbar = new QStatusBar(wondronDlg);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        wondronDlg->setStatusBar(statusbar);

        retranslateUi(wondronDlg);

        QMetaObject::connectSlotsByName(wondronDlg);
    } // setupUi

    void retranslateUi(QMainWindow *wondronDlg)
    {
        wondronDlg->setWindowTitle(QCoreApplication::translate("wondronDlg", "wondronDlg", nullptr));
        groupBox->setTitle(QCoreApplication::translate("wondronDlg", "GroupBox", nullptr));
    } // retranslateUi

};

namespace Ui {
    class wondronDlg: public Ui_wondronDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WONDRONDLG_H
