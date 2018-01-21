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
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>
#include <QVBoxLayout>
#include <QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
	QTableWidget *devicetable;
	QVBoxLayout * main_layout;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QStringLiteral("Form"));
        Form->resize(658, 431);
        devicetable = new QTableWidget(Form);
		main_layout = new QVBoxLayout;
        devicetable->setObjectName(QStringLiteral("devicetable"));
        //devicetable->setGeometry(QRect(0, 0, 651, 431));

		main_layout->addWidget(devicetable);

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
