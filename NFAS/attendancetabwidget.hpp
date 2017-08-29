#ifndef ATTENDANCETABWIDGET_H
#define ATTENDANCETABWIDGET_H
#include <QWidget>
#include "DataManager.h"
#include <QTreeWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QDateTimeEdit>

class AttendanceTabWidget : public QWidget {
	Q_OBJECT

public:
	AttendanceTabWidget(DataManager *dm, QWidget * parent = 0);
	~AttendanceTabWidget();

	void UpdateTab();

public slots:
	void AttendanceAdd();
	void AttendanceModify();
	void AttendanceDelete();
	void updateTable(QTreeWidgetItem* item, int col);
	void updateAttendanceClass(QTreeWidgetItem* item, int col);
	
private:
	void setupUi();
	void updateTree();

	DataManager* dataManager;
	QTreeWidget* attendancetree;
	QTreeWidget* classtree;
	QTableWidget* attendancetable;
	QLineEdit* attendanceid;
	QLineEdit* attendancename;
	QLineEdit* attendanceclassroomid;
	QDateTimeEdit* attendancestarttime;
	QDateTimeEdit* attendanceendtime;
	QLineEdit* attendanceclass;
	QPushButton* attendanceadd;
	QPushButton* attendancemodify;
	QPushButton* attendancedelete;
};
#endif // ATTENDANCETABWIDGET_H