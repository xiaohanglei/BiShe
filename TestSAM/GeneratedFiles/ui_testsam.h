/********************************************************************************
** Form generated from reading UI file 'testsam.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TESTSAM_H
#define UI_TESTSAM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TestSAMClass
{
public:

    void setupUi(QWidget *TestSAMClass)
    {
        if (TestSAMClass->objectName().isEmpty())
            TestSAMClass->setObjectName(QStringLiteral("TestSAMClass"));
        TestSAMClass->resize(600, 400);

        retranslateUi(TestSAMClass);

        QMetaObject::connectSlotsByName(TestSAMClass);
    } // setupUi

    void retranslateUi(QWidget *TestSAMClass)
    {
        TestSAMClass->setWindowTitle(QApplication::translate("TestSAMClass", "TestSAM", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class TestSAMClass: public Ui_TestSAMClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TESTSAM_H
