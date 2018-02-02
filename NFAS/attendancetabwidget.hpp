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
#include <QCheckBox>
#include <QTimeEdit>

struct ATTENDANCETIME//考勤时段
{
	QString time1start;
	QString time1end;

	QString time2start;
	QString time2end;

	QString time3start;
	QString time3end;

	QString time4start;
	QString time4end;

	QString time5start;
	QString time5end;

};



class AttendanceTabWidget : public QWidget
{
	Q_OBJECT

public:
	AttendanceTabWidget(DataManager *dm, QWidget * parent = 0);
	~AttendanceTabWidget();

	bool AttendanceTimePutDB(QString aid, ATTENDANCETIME monday_attend, ATTENDANCETIME Tuesday_attend, ATTENDANCETIME Wednesday_attend, ATTENDANCETIME Thursday_attend, ATTENDANCETIME Friday_attend);

	void RestAttendanceTime();

	void UpdateTab();

public slots:
	void AttendanceAdd();
	void AttendanceModify();
	void AttendanceDelete();
	void updateTable(QTreeWidgetItem* item, int col);
	void updateAttendanceClass(QTreeWidgetItem* item, int col);
	void updateAttendanceLeader(QTreeWidgetItem * item, int col);//更新考勤班级

	void ChangeCheckMonday(int state);
	void ChangeCheckTuesday (int state);
	void ChangeCheckWednesday(int state);
	void ChangeCheckThursday (int state);
	void ChangeCheckFriday(int state);
	void ChangeCheckSaturday (int state);
	void ChangeCheckSunday(int state);

	
private:
	void setupUi();
	void updateTree();

	DataManager* dataManager;
	QTreeWidget* attendancetree;
	QTreeWidget* classtree;

	QTreeWidget * leadertree;

	QTableWidget* attendancetable;
	QLineEdit* attendanceid;
	QLineEdit* attendancename;
	QLineEdit* attendanceclassroomid;
	QDateTimeEdit* attendancestarttime;
	QDateTimeEdit* attendanceendtime;
	QLineEdit * attendanceleader;
	QLineEdit* attendanceclass;
	QPushButton* attendanceadd;
	QPushButton* attendancemodify;
	QPushButton* attendancedelete;

	//考勤时段
	//Thursday, Friday, Saturday, Sunday.
	QCheckBox * checkmonday;
	QCheckBox * checkTuesday;
	QCheckBox * checkWednesday;
	QCheckBox * checkThursday;
	QCheckBox * checkFriday;
	QCheckBox * checkSaturday;
	QCheckBox * checkSunday;

	//Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday.

	QCheckBox * time1monday;
	QTimeEdit * starttimemonday1;
	QTimeEdit * endtimemonday1;
	QCheckBox * time2monday;
	QTimeEdit * starttimemonday2;
	QTimeEdit * endtimemonday2;
	QCheckBox * time3monday;
	QTimeEdit * starttimemonday3;
	QTimeEdit * endtimemonday3;
	QCheckBox * time4monday;
	QTimeEdit * starttimemonday4;
	QTimeEdit * endtimemonday4;
	QCheckBox * time5monday;
	QTimeEdit * starttimemonday5;
	QTimeEdit * endtimemonday5;


	QCheckBox * time1Tuesday;
	QTimeEdit * starttimeTuesday1;
	QTimeEdit * endtimeTuesday1;
	QCheckBox * time2Tuesday;
	QTimeEdit * starttimeTuesday2;
	QTimeEdit * endtimeTuesday2;
	QCheckBox * time3Tuesday;
	QTimeEdit * starttimeTuesday3;
	QTimeEdit * endtimeTuesday3;
	QCheckBox * time4Tuesday;
	QTimeEdit * starttimeTuesday4;
	QTimeEdit * endtimeTuesday4;
	QCheckBox * time5Tuesday;
	QTimeEdit * starttimeTuesday5;
	QTimeEdit * endtimeTuesday5;

	QCheckBox * time1Wednesday;
	QTimeEdit * starttimeWednesday1;
	QTimeEdit * endtimeWednesday1;
	QCheckBox * time2Wednesday;
	QTimeEdit * starttimeWednesday2;
	QTimeEdit * endtimeWednesday2;
	QCheckBox * time3Wednesday;
	QTimeEdit * starttimeWednesday3;
	QTimeEdit * endtimeWednesday3;
	QCheckBox * time4Wednesday;
	QTimeEdit * starttimeWednesday4;
	QTimeEdit * endtimeWednesday4;
	QCheckBox * time5Wednesday;
	QTimeEdit * starttimeWednesday5;
	QTimeEdit * endtimeWednesday5;

	QCheckBox * time1Thursday;
	QTimeEdit * starttimeThursday1;
	QTimeEdit * endtimeThursday1;
	QCheckBox * time2Thursday;
	QTimeEdit * starttimeThursday2;
	QTimeEdit * endtimeThursday2;
	QCheckBox * time3Thursday;
	QTimeEdit * starttimeThursday3;
	QTimeEdit * endtimeThursday3;
	QCheckBox * time4Thursday;
	QTimeEdit * starttimeThursday4;
	QTimeEdit * endtimeThursday4;
	QCheckBox * time5Thursday;
	QTimeEdit * starttimeThursday5;
	QTimeEdit * endtimeThursday5;

	QCheckBox * time1Friday;
	QTimeEdit * starttimeFriday1;
	QTimeEdit * endtimeFriday1;
	QCheckBox * time2Friday;
	QTimeEdit * starttimeFriday2;
	QTimeEdit * endtimeFriday2;
	QCheckBox * time3Friday;
	QTimeEdit * starttimeFriday3;
	QTimeEdit * endtimeFriday3;
	QCheckBox * time4Friday;
	QTimeEdit * starttimeFriday4;
	QTimeEdit * endtimeFriday4;
	QCheckBox * time5Friday;
	QTimeEdit * starttimeFriday5;
	QTimeEdit * endtimeFriday5;




};
#endif // ATTENDANCETABWIDGET_H