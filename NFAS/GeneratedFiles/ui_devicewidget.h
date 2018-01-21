/********************************************************************************
** Form generated from reading UI file 'devicewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEVICEWIDGET_H
#define UI_DEVICEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QVBoxLayout *verticalLayout;
    QTableWidget *devicetable;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QStringLiteral("Form"));
        Form->resize(851, 590);
        verticalLayout = new QVBoxLayout(Form);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        devicetable = new QTableWidget(Form);
        if (devicetable->columnCount() < 2)
            devicetable->setColumnCount(2);
        devicetable->setObjectName(QStringLiteral("devicetable"));
        QFont font;
        font.setPointSize(10);
        devicetable->setFont(font);
        devicetable->setLineWidth(5);
        devicetable->setGridStyle(Qt::DashLine);
        devicetable->setColumnCount(2);
        devicetable->horizontalHeader()->setCascadingSectionResizes(true);

        verticalLayout->addWidget(devicetable);


        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEVICEWIDGET_H
