/********************************************************************************
** Form generated from reading UI file 'attendance.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ATTENDANCE_H
#define UI_ATTENDANCE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Attendance
{
public:

    void setupUi(QWidget *Attendance)
    {
        if (Attendance->objectName().isEmpty())
            Attendance->setObjectName(QStringLiteral("Attendance"));
        Attendance->resize(400, 300);

        retranslateUi(Attendance);

        QMetaObject::connectSlotsByName(Attendance);
    } // setupUi

    void retranslateUi(QWidget *Attendance)
    {
        Attendance->setWindowTitle(QApplication::translate("Attendance", "Attendance", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Attendance: public Ui_Attendance {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ATTENDANCE_H
