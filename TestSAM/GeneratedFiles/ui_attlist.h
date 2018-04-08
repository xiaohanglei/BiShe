/********************************************************************************
** Form generated from reading UI file 'attlist.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ATTLIST_H
#define UI_ATTLIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_attlist
{
public:

    void setupUi(QWidget *attlist)
    {
        if (attlist->objectName().isEmpty())
            attlist->setObjectName(QStringLiteral("attlist"));
        attlist->resize(400, 300);

        retranslateUi(attlist);

        QMetaObject::connectSlotsByName(attlist);
    } // setupUi

    void retranslateUi(QWidget *attlist)
    {
        attlist->setWindowTitle(QApplication::translate("attlist", "attlist", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class attlist: public Ui_attlist {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ATTLIST_H
