#include "attendancetabwidget.hpp"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QMessageBox>
#include <QDateTime>
#include <QHeaderView>

AttendanceTabWidget::AttendanceTabWidget(DataManager *dm, QWidget * parent) : QWidget(parent) 
{
	dataManager = dm;
	setupUi();

	connect(attendanceadd, SIGNAL(clicked()), this, SLOT(AttendanceAdd()));
	connect(attendancemodify, SIGNAL(clicked()), this, SLOT(AttendanceModify()));
	connect(attendancedelete, SIGNAL(clicked()), this, SLOT(AttendanceDelete()));
	connect(attendancetree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(updateTable(QTreeWidgetItem*, int)));
	connect(classtree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(updateAttendanceClass(QTreeWidgetItem*, int)));

	connect(checkmonday, SIGNAL(stateChanged(int)), this, SLOT(ChangeCheckMonday(int)));
	connect(checkTuesday, SIGNAL(stateChanged(int)), this, SLOT(ChangeCheckTuesday(int)));
	connect(checkWednesday, SIGNAL(stateChanged(int)), this, SLOT(ChangeCheckWednesday(int)));
	connect(checkThursday, SIGNAL(stateChanged(int)), this, SLOT(ChangeCheckThursday(int)));
	connect(checkFriday, SIGNAL(stateChanged(int)), this, SLOT(ChangeCheckFriday(int)));
	connect(checkSaturday, SIGNAL(stateChanged(int)), this, SLOT(ChangeCheckSaturday(int)));
	connect(checkSunday, SIGNAL(stateChanged(int)), this, SLOT(ChangeCheckSunday(int)));
}

AttendanceTabWidget::~AttendanceTabWidget() 
{
	
}

void AttendanceTabWidget::UpdateTab()
{
}

void AttendanceTabWidget::AttendanceModify()
{
	auto items = attendancetree->selectedItems();
	if (items.length() != 1) 
	{
		QMessageBox::information(0, tr("attendance modify"), tr("please select attendance first"), QMessageBox::Ok);
		return;
	}
	if (attendanceclass->text().split(",").length() == 0) 
	{
		QMessageBox::information(0, tr("attendance modify"), tr("please select attendance class first"), QMessageBox::Ok);
		return;
	}
	QString aid = items[0]->text(0).split("-")[0];

	Attendance attendance(aid, attendanceclassroomid->text().trimmed(), attendancename->text().trimmed(), attendancestarttime->dateTime(), attendanceendtime->dateTime(), attendanceclass->text());
	if (dataManager->AttendanceOP(attendance, 1)) 
	{
		QMessageBox::information(0, tr("attendance modify"), tr("attendance modify successfully"), QMessageBox::Ok);
		attendanceid->setText("");
		attendancename->setText("");
		attendanceclassroomid->setText("");
		attendanceclass->setText("");
		dataManager->updateAttendance();
		updateTree();//更新树形空间
	}
	else 
	{
		QMessageBox::information(0, tr("attendance modify"), tr("attendance modify failed"), QMessageBox::Ok);
	}
}

void AttendanceTabWidget::AttendanceDelete()
{
	auto items = attendancetree->selectedItems();
	if (items.size() == 1) 
	{
		auto aid = items[0]->text(0).split("-")[0];
		if (dataManager->AttendanceOP(Attendance(aid, "", "", "", ""), 2))
		{
			QMessageBox::information(0, tr("attendance delete"), tr("attendance delete successfully"), QMessageBox::Ok);


			attendanceid->setText("");
			attendancename->setText("");
			attendanceclassroomid->setText("");
			attendanceclass->setText("");

			dataManager->updateResult(dataManager->GetCurrentUser());
			dataManager->updateAttendance();
			updateTree();//更新树形空间

		}
		else
		{
			QMessageBox::information(0, tr("attendance delete"), tr("attendance delete failed"), QMessageBox::Ok);
		}
	}
	else 
	{
		QMessageBox::information(0, tr("attendance delete"), tr("please select attendance first"), QMessageBox::Ok);
	}
	
}

void AttendanceTabWidget::updateTable(QTreeWidgetItem * item, int col)
{
	attendancetable->clear();
	attendancetable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	attendancetable->setRowCount(0);
	attendancetable->setColumnCount(6);
	QStringList header;
	header << tr("student id") << tr("student name") << tr("student sex") << tr("student academic") << tr("student class") << tr("student figure");
	attendancetable->setHorizontalHeaderLabels(header);
	auto s = item->text(0).split("-")[0];
	for (auto it = dataManager->GetAttendance()->begin(); it != dataManager->GetAttendance()->end(); it++) 
	{
		if (it->GetID() == s) 
		{
			//
			attendanceid->setText(it->GetID());
			attendancename->setText(it->GetName());
			attendanceclassroomid->setText(it->GetMID());
			attendanceclass->setText(it->GetAclass());

			QString satrttime = it->GetSETime().split("-").first();
			QString endtime = it->GetSETime().split("-").last();
			attendancestarttime->setDateTime(QDateTime::fromString(satrttime, "yyyy/MM/dd hh:mm"));
			attendanceendtime->setDateTime(QDateTime::fromString(endtime, "yyyy/MM/dd hh:mm"));

			QStringList aclass = it->GetAclass().split(",");
			for (int i = 0; i < aclass.size(); i++) 
			{
				auto acla = aclass.at(i);
				for (auto iter = dataManager->GetStudent()->begin(); iter != dataManager->GetStudent()->end(); iter++) 
				{
					if (iter->GetAclass() == acla) 
					{
						auto rowcount = attendancetable->rowCount();
						attendancetable->insertRow(rowcount);
						attendancetable->setItem(rowcount, 0, new QTableWidgetItem(iter->GetID()));
						attendancetable->setItem(rowcount, 1, new QTableWidgetItem(iter->GetName()));
						attendancetable->setItem(rowcount, 2, new QTableWidgetItem((iter->GetSex() == 0) ? tr("male") : tr("female")));
						attendancetable->setItem(rowcount, 3, new QTableWidgetItem(iter->GetAcademic()));
						attendancetable->setItem(rowcount, 4, new QTableWidgetItem(iter->GetAclass()));
						attendancetable->setItem(rowcount, 5, new QTableWidgetItem(iter->GetFigure()));
					}
				}
			}
			break;
		}
	}
	for (int i = 0; i < attendancetable->rowCount(); i++)
	{
		attendancetable->item(i, 0)->setFlags(attendancetable->item(i, 0)->flags() & (~Qt::ItemIsEditable));
		attendancetable->item(i, 1)->setFlags(attendancetable->item(i, 1)->flags() & (~Qt::ItemIsEditable));
		attendancetable->item(i, 2)->setFlags(attendancetable->item(i, 2)->flags() & (~Qt::ItemIsEditable));
		attendancetable->item(i, 3)->setFlags(attendancetable->item(i, 3)->flags() & (~Qt::ItemIsEditable));
		attendancetable->item(i, 4)->setFlags(attendancetable->item(i, 4)->flags() & (~Qt::ItemIsEditable));
		attendancetable->item(i, 5)->setFlags(attendancetable->item(i, 5)->flags() & (~Qt::ItemIsEditable));
	}

	//




}

void AttendanceTabWidget::updateAttendanceClass(QTreeWidgetItem * item, int col)//更新考勤班级
{
	QStringList aclass;
	if (attendanceclass->text().trimmed().length() > 0) 
	{
		aclass = attendanceclass->text().trimmed().split(",");
	}
	auto sclassid = item->text(0).split("-")[0];
	auto index = aclass.indexOf(sclassid);
	if (index >= 0) 
	{
		aclass.removeAt(index);
	}
	else 
	{
		aclass.append(sclassid);
	}
	attendanceclass->setText(aclass.join(","));
}
//Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday

void AttendanceTabWidget::ChangeCheckMonday(int state)
{
	if (state == 2)//选中
	{
		time1monday->setEnabled(true);
		starttimemonday1->setEnabled(true);
		endtimemonday1->setEnabled(true);

		time2monday->setEnabled(true);
		starttimemonday2->setEnabled(true);
		endtimemonday2->setEnabled(true);
		
		time3monday->setEnabled(true);
		starttimemonday3->setEnabled(true);
		endtimemonday3->setEnabled(true);

		time4monday->setEnabled(true);
		starttimemonday4->setEnabled(true);
		endtimemonday4->setEnabled(true);

		time5monday->setEnabled(true);
		starttimemonday5->setEnabled(true);
		endtimemonday5->setEnabled(true);

	}
	else if (state == 0)//取消
	{
		time1monday->setEnabled(false);
		starttimemonday1->setEnabled(false);
		endtimemonday1->setEnabled(false);

		time2monday->setEnabled(false);
		starttimemonday2->setEnabled(false);
		endtimemonday2->setEnabled(false);

		time3monday->setEnabled(false);
		starttimemonday3->setEnabled(false);
		endtimemonday3->setEnabled(false);

		time4monday->setEnabled(false);
		starttimemonday4->setEnabled(false);
		endtimemonday4->setEnabled(false);

		time5monday->setEnabled(false);
		starttimemonday5->setEnabled(false);
		endtimemonday5->setEnabled(false);
	}
}

void AttendanceTabWidget::ChangeCheckTuesday(int state)
{
	if (state == 2)//选中
	{
		time1Tuesday->setEnabled(true);
		starttimeTuesday1->setEnabled(true);
		endtimeTuesday1->setEnabled(true);

		time2Tuesday->setEnabled(true);
		starttimeTuesday2->setEnabled(true);
		endtimeTuesday2->setEnabled(true);

		time3Tuesday->setEnabled(true);
		starttimeTuesday3->setEnabled(true);
		endtimeTuesday3->setEnabled(true);

		time4Tuesday->setEnabled(true);
		starttimeTuesday4->setEnabled(true);
		endtimeTuesday4->setEnabled(true);

		time5Tuesday->setEnabled(true);
		starttimeTuesday5->setEnabled(true);
		endtimeTuesday5->setEnabled(true);

	}
	else if (state == 0)//取消
	{
		time1Tuesday->setEnabled(false);
		starttimeTuesday1->setEnabled(false);
		endtimeTuesday1->setEnabled(false);

		time2Tuesday->setEnabled(false);
		starttimeTuesday2->setEnabled(false);
		endtimeTuesday2->setEnabled(false);

		time3Tuesday->setEnabled(false);
		starttimeTuesday3->setEnabled(false);
		endtimeTuesday3->setEnabled(false);

		time4Tuesday->setEnabled(false);
		starttimeTuesday4->setEnabled(false);
		endtimeTuesday4->setEnabled(false);

		time5Tuesday->setEnabled(false);
		starttimeTuesday5->setEnabled(false);
		endtimeTuesday5->setEnabled(false);
	}
}

void AttendanceTabWidget::ChangeCheckWednesday(int state)
{
	if (state == 2)//选中
	{
		time1Wednesday->setEnabled(true);
		starttimeWednesday1->setEnabled(true);
		endtimeWednesday1->setEnabled(true);

		time2Wednesday->setEnabled(true);
		starttimeWednesday2->setEnabled(true);
		endtimeWednesday2->setEnabled(true);

		time3Wednesday->setEnabled(true);
		starttimeWednesday3->setEnabled(true);
		endtimeWednesday3->setEnabled(true);

		time4Wednesday->setEnabled(true);
		starttimeWednesday4->setEnabled(true);
		endtimeWednesday4->setEnabled(true);

		time5Wednesday->setEnabled(true);
		starttimeWednesday5->setEnabled(true);
		endtimeWednesday5->setEnabled(true);

	}
	else if (state == 0)//取消
	{
		time1Wednesday->setEnabled(false);
		starttimeWednesday1->setEnabled(false);
		endtimeWednesday1->setEnabled(false);

		time2Wednesday->setEnabled(false);
		starttimeWednesday2->setEnabled(false);
		endtimeWednesday2->setEnabled(false);

		time3Wednesday->setEnabled(false);
		starttimeWednesday3->setEnabled(false);
		endtimeWednesday3->setEnabled(false);

		time4Wednesday->setEnabled(false);
		starttimeWednesday4->setEnabled(false);
		endtimeWednesday4->setEnabled(false);

		time5Wednesday->setEnabled(false);
		starttimeWednesday5->setEnabled(false);
		endtimeWednesday5->setEnabled(false);
	}
}

void AttendanceTabWidget::ChangeCheckThursday(int state)
{
	if (state == 2)//选中
	{
		time1Thursday->setEnabled(true);
		starttimeThursday1->setEnabled(true);
		endtimeThursday1->setEnabled(true);

		time2Thursday->setEnabled(true);
		starttimeThursday2->setEnabled(true);
		endtimeThursday2->setEnabled(true);

		time3Thursday->setEnabled(true);
		starttimeThursday3->setEnabled(true);
		endtimeThursday3->setEnabled(true);

		time4Thursday->setEnabled(true);
		starttimeThursday4->setEnabled(true);
		endtimeThursday4->setEnabled(true);

		time5Thursday->setEnabled(true);
		starttimeThursday5->setEnabled(true);
		endtimeThursday5->setEnabled(true);

	}
	else if (state == 0)//取消
	{
		time1Thursday->setEnabled(false);
		starttimeThursday1->setEnabled(false);
		endtimeThursday1->setEnabled(false);

		time2Thursday->setEnabled(false);
		starttimeThursday2->setEnabled(false);
		endtimeThursday2->setEnabled(false);

		time3Thursday->setEnabled(false);
		starttimeThursday3->setEnabled(false);
		endtimeThursday3->setEnabled(false);

		time4Thursday->setEnabled(false);
		starttimeThursday4->setEnabled(false);
		endtimeThursday4->setEnabled(false);

		time5Thursday->setEnabled(false);
		starttimeThursday5->setEnabled(false);
		endtimeThursday5->setEnabled(false);
	}
}

void AttendanceTabWidget::ChangeCheckFriday(int state)
{
	if (state == 2)//选中
	{
		time1Friday->setEnabled(true);
		starttimeFriday1->setEnabled(true);
		endtimeFriday1->setEnabled(true);

		time2Friday->setEnabled(true);
		starttimeFriday2->setEnabled(true);
		endtimeFriday2->setEnabled(true);

		time3Friday->setEnabled(true);
		starttimeFriday3->setEnabled(true);
		endtimeFriday3->setEnabled(true);

		time4Friday->setEnabled(true);
		starttimeFriday4->setEnabled(true);
		endtimeFriday4->setEnabled(true);

		time5Friday->setEnabled(true);
		starttimeFriday5->setEnabled(true);
		endtimeFriday5->setEnabled(true);

	}
	else if (state == 0)//取消
	{
		time1Friday->setEnabled(false);
		starttimeFriday1->setEnabled(false);
		endtimeFriday1->setEnabled(false);

		time2Friday->setEnabled(false);
		starttimeFriday2->setEnabled(false);
		endtimeFriday2->setEnabled(false);

		time3Friday->setEnabled(false);
		starttimeFriday3->setEnabled(false);
		endtimeFriday3->setEnabled(false);

		time4Friday->setEnabled(false);
		starttimeFriday4->setEnabled(false);
		endtimeFriday4->setEnabled(false);

		time5Friday->setEnabled(false);
		starttimeFriday5->setEnabled(false);
		endtimeFriday5->setEnabled(false);
	}
}

void AttendanceTabWidget::ChangeCheckSaturday(int state)
{
	if (state == 2)//选中
	{
	
	}
	else if (state == 0)//取消
	{
		
	}
}

void AttendanceTabWidget::ChangeCheckSunday(int state)
{
	if (state == 2)//选中
	{
		

	}
	else if (state == 0)//取消
	{
		
	}
}



void AttendanceTabWidget::AttendanceAdd()
{
	QString aid = attendanceid->text().trimmed();
	QString aname = attendancename->text().trimmed();
	QString amid = attendanceclassroomid->text().trimmed();
	QDateTime starttime = attendancestarttime->dateTime();
	QDateTime endtime = attendanceendtime->dateTime();
	QString aclass = attendanceclass->text().trimmed();
	if (aid.length() != 8) 
	{
		QMessageBox::information(0, tr("attendance add"), tr("length of attendance id must be 8"), QMessageBox::Ok);
		return;
	}
	if (amid.length() != 6) 
	{
		QMessageBox::information(0, tr("attendance add"), tr("length of attendance classroom id must be 6"), QMessageBox::Ok);
		return;
	}
	if (!(aname.length() > 0)) 
	{
		QMessageBox::information(0, tr("attendance add"), tr("attendance name is required"), QMessageBox::Ok);
		return;
	}
	if (!(aclass.length() > 0)) 
	{
		QMessageBox::information(0, tr("attendance add"), tr("please select attendance class"), QMessageBox::Ok);
		return;
	}
	QDateTime datetime;
	auto now = datetime.currentDateTime();
	//必须提前10分钟以上发送开始考勤时间
	if (starttime.toTime_t() - now.toTime_t() < 10 * 60) 
	{
		//考勤开始时间必须比当前时间晚10分钟以上
		QMessageBox::information(0, tr("attendance add"), tr("start time must later than now more than 10 mins"), QMessageBox::Ok);
		return;
	}
	if (endtime.toTime_t() - starttime.toTime_t() < 30 * 60)//考勤时长不得低于30分钟
	{
		QMessageBox::information(0, tr("attendance add"), tr("end time must later than start time more than 30 mins"), QMessageBox::Ok);
		return;
	}
	
	Attendance attendance(aid, amid, aname, starttime, endtime, aclass);
	if (dataManager->AttendanceOP(attendance, 0)) 
	{
		QMessageBox::information(0, tr("attendance add"), tr("attendance add successfully"), QMessageBox::Ok);
		attendanceid->setText("");
		attendancename->setText("");
		attendanceclassroomid->setText("");
		attendanceclass->setText("");
		dataManager->updateAttendance();
		updateTree();
	}
	else
	{
		QMessageBox::information(0, tr("academic add"), tr("academic add failed"), QMessageBox::Ok);
	}
}

void AttendanceTabWidget::setupUi()
{
	QVBoxLayout* leftlayout = new QVBoxLayout;
	attendancetree = new QTreeWidget;
	attendancetree->setColumnCount(1);
	attendancetree->setMaximumWidth(200);
	attendancetree->setHeaderLabel(tr("attendance information"));
	leftlayout->addWidget(attendancetree);

	QVBoxLayout* rightlayout = new QVBoxLayout;
	QHBoxLayout* rightclasslayout = new QHBoxLayout;
	attendancetable = new QTableWidget;
	rightclasslayout->addWidget(attendancetable);
	classtree = new QTreeWidget;
	classtree->setColumnCount(1);
	classtree->setMaximumWidth(150);
	classtree->setHeaderLabel(tr("class information"));
	rightclasslayout->addWidget(classtree);
	rightlayout->addLayout(rightclasslayout);
	QGroupBox* attendancegroup = new QGroupBox(tr("attendance information administrate"));
	QVBoxLayout* attendacelayout = new QVBoxLayout;

	QHBoxLayout* attendacemanagerlayout = new QHBoxLayout;

	QHBoxLayout* attendanceinfo1 = new QHBoxLayout;
	QHBoxLayout* attendanceinfo2 = new QHBoxLayout;
	QHBoxLayout* attendanceinfo3 = new QHBoxLayout;
	QLabel* labelattendaceid = new QLabel(tr("attendance id"));
	QLabel* labelattendancename = new QLabel(tr("attendance name"));
	QLabel* labelattendanceclassroomid = new QLabel(tr("classroom id"));
	QLabel* labelstarttime = new QLabel(tr("start time"));
	QLabel* labelendtime = new QLabel(tr("end time"));
	QLabel* labelclass = new QLabel(tr("attendance class"));
	attendanceid = new QLineEdit;
	attendanceid->setMaxLength(8);
	attendancename = new QLineEdit;
	attendanceclassroomid = new QLineEdit;
	attendanceclassroomid->setMaxLength(6);
	QDateTime datetime;
	auto current = datetime.currentDateTime();
	attendancestarttime = new QDateTimeEdit;
	attendancestarttime->setDisplayFormat("yyyy/MM/dd HH:mm");
	attendancestarttime->setDateTime(current);
	attendancestarttime->setCalendarPopup(true);
	attendanceendtime = new QDateTimeEdit;
	attendanceendtime->setDisplayFormat("yyyy/MM/dd HH:mm");
	attendanceendtime->setDateTime(current.addSecs(45*60));
	attendanceendtime->setCalendarPopup(true);
	attendanceclass = new QLineEdit;
	attendanceclass->setEnabled(false);
	attendanceinfo1->addWidget(labelattendaceid);
	attendanceinfo1->addWidget(attendanceid);
	attendanceinfo1->addStretch(1);
	attendanceinfo1->addWidget(labelattendancename);
	attendanceinfo1->addWidget(attendancename);
	attendanceinfo1->addStretch(1);
	attendanceinfo1->addWidget(labelattendanceclassroomid);
	attendanceinfo1->addWidget(attendanceclassroomid);
	attendanceinfo2->addWidget(labelstarttime);
	attendanceinfo2->addWidget(attendancestarttime);
	attendanceinfo2->addStretch(1);
	attendanceinfo2->addWidget(labelendtime);
	attendanceinfo2->addWidget(attendanceendtime);
	attendanceinfo2->addStretch(2);
	attendanceinfo3->addWidget(labelclass);
	attendanceinfo3->addWidget(attendanceclass);
	attendacelayout->addLayout(attendanceinfo1);
	attendacelayout->addLayout(attendanceinfo2);
	attendacelayout->addLayout(attendanceinfo3);
	attendanceadd = new QPushButton(tr("attendance add"));
	attendancemodify = new QPushButton(tr("atendance modify"));
	attendancedelete = new QPushButton(tr("attendance delete"));
	attendanceadd->setMinimumWidth(120);
	attendancemodify->setMinimumWidth(120);
	attendancedelete->setMinimumWidth(120);
	QHBoxLayout* attendanceop = new QHBoxLayout;
	attendanceop->addStretch(1);
	attendanceop->addWidget(attendanceadd);
	attendanceop->addWidget(attendancemodify);
	attendanceop->addWidget(attendancedelete);
	
	attendacelayout->addLayout(attendanceop);
	attendancegroup->setLayout(attendacelayout);

	
	//rightlayout->addWidget(attendancegroup);
	//rightlayout->addStretch(10);
	//------考勤时段

	QVBoxLayout * mondaylayout		= new QVBoxLayout;
	QVBoxLayout * Tuesdaylayout		= new QVBoxLayout;
	QVBoxLayout * Wednesdaylayout	= new QVBoxLayout;
	QVBoxLayout * Thursdaylayout	 = new QVBoxLayout;
	QVBoxLayout * Fridaylayout		= new QVBoxLayout;
	QVBoxLayout * Saturdaylayout	= new QVBoxLayout;
	QVBoxLayout * Sundaylayout		= new QVBoxLayout;
	


	checkmonday		= new QCheckBox(tr("Monday"));
	
	time1monday = new QCheckBox(tr("time1"));
	starttimemonday1 = new QTimeEdit;
	endtimemonday1 = new QTimeEdit;
	time2monday = new QCheckBox(tr("time2"));
	starttimemonday2 = new QTimeEdit;
	endtimemonday2 = new QTimeEdit;
	time3monday = new QCheckBox(tr("time3"));
	starttimemonday3 = new QTimeEdit;
	endtimemonday3 = new QTimeEdit;
	time4monday = new QCheckBox(tr("time4"));
	starttimemonday4 = new QTimeEdit;
	endtimemonday4 = new QTimeEdit;
	time5monday = new QCheckBox(tr("time5"));
	starttimemonday5 = new QTimeEdit;
	endtimemonday5 = new QTimeEdit;
		

	checkTuesday	   = new QCheckBox(tr("Tuesday"));
	time1Tuesday = new QCheckBox(tr("time1"));
	starttimeTuesday1 = new QTimeEdit;
	endtimeTuesday1 = new QTimeEdit;
	time2Tuesday = new QCheckBox(tr("time2"));
	starttimeTuesday2 = new QTimeEdit;
	endtimeTuesday2 = new QTimeEdit;
	time3Tuesday = new QCheckBox(tr("time3"));
	starttimeTuesday3 = new QTimeEdit;
	endtimeTuesday3 = new QTimeEdit;
	time4Tuesday = new QCheckBox(tr("time4"));
	starttimeTuesday4 = new QTimeEdit;
	endtimeTuesday4 = new QTimeEdit;
	time5Tuesday = new QCheckBox(tr("time5"));
	starttimeTuesday5 = new QTimeEdit;
	endtimeTuesday5 = new QTimeEdit;


	checkWednesday	   = new QCheckBox(tr("Wednesday"));
	time1Wednesday = new QCheckBox(tr("time1"));
	starttimeWednesday1 = new QTimeEdit;
	endtimeWednesday1 = new QTimeEdit;
	time2Wednesday = new QCheckBox(tr("time2"));
	starttimeWednesday2 = new QTimeEdit;
	endtimeWednesday2 = new QTimeEdit;
	time3Wednesday = new QCheckBox(tr("time3"));
	starttimeWednesday3 = new QTimeEdit;
	endtimeWednesday3 = new QTimeEdit;
	time4Wednesday = new QCheckBox(tr("time4"));
	starttimeWednesday4 = new QTimeEdit;
	endtimeWednesday4 = new QTimeEdit;
	time5Wednesday = new QCheckBox(tr("time5"));
	starttimeWednesday5 = new QTimeEdit;
	endtimeWednesday5 = new QTimeEdit;



	checkThursday	   = new QCheckBox(tr("Thursday"));
	time1Thursday = new QCheckBox(tr("time1"));
	starttimeThursday1 = new QTimeEdit;
	endtimeThursday1 = new QTimeEdit;
	time2Thursday = new QCheckBox(tr("time2"));
	starttimeThursday2 = new QTimeEdit;
	endtimeThursday2 = new QTimeEdit;
	time3Thursday = new QCheckBox(tr("time3"));
	starttimeThursday3 = new QTimeEdit;
	endtimeThursday3 = new QTimeEdit;
	time4Thursday = new QCheckBox(tr("time4"));
	starttimeThursday4 = new QTimeEdit;
	endtimeThursday4 = new QTimeEdit;
	time5Thursday = new QCheckBox(tr("time5"));
	starttimeThursday5 = new QTimeEdit;
	endtimeThursday5 = new QTimeEdit;


	checkFriday		= new QCheckBox(tr("Friday"));
	time1Friday = new QCheckBox(tr("time1"));
	starttimeFriday1 = new QTimeEdit;
	endtimeFriday1 = new QTimeEdit;
	time2Friday = new QCheckBox(tr("time2"));
	starttimeFriday2 = new QTimeEdit;
	endtimeFriday2 = new QTimeEdit;
	time3Friday = new QCheckBox(tr("time3"));
	starttimeFriday3 = new QTimeEdit;
	endtimeFriday3 = new QTimeEdit;
	time4Friday = new QCheckBox(tr("time4"));
	starttimeFriday4 = new QTimeEdit;
	endtimeFriday4 = new QTimeEdit;
	time5Friday = new QCheckBox(tr("time5"));
	starttimeFriday5 = new QTimeEdit;
	endtimeFriday5 = new QTimeEdit;


	checkSaturday   = new QCheckBox(tr("Saturday"));
	checkSunday		= new QCheckBox(tr("Sunday"));

	//设置默认选中状态
	checkmonday->setCheckState(Qt::CheckState(2));
	checkTuesday->setCheckState(Qt::CheckState(2));
	checkWednesday->setCheckState(Qt::CheckState(2));
	checkThursday->setCheckState(Qt::CheckState(2));
	checkFriday->setCheckState(Qt::CheckState(2));

	//周一

	mondaylayout->addWidget(checkmonday);

	QHBoxLayout * mondaytime1layout = new QHBoxLayout;
	mondaytime1layout->addWidget(time1monday);
	mondaytime1layout->addWidget(starttimemonday1);
	mondaytime1layout->addWidget(endtimemonday1);

	QHBoxLayout * mondaytime2layout = new QHBoxLayout;
	mondaytime2layout->addWidget(time2monday);
	mondaytime2layout->addWidget(starttimemonday2);
	mondaytime2layout->addWidget(endtimemonday2);

	QHBoxLayout * mondaytime3layout = new QHBoxLayout;
	mondaytime3layout->addWidget(time3monday);
	mondaytime3layout->addWidget(starttimemonday3);
	mondaytime3layout->addWidget(endtimemonday3);

	QHBoxLayout * mondaytime4layout = new QHBoxLayout;
	mondaytime4layout->addWidget(time4monday);
	mondaytime4layout->addWidget(starttimemonday4);
	mondaytime4layout->addWidget(endtimemonday4);

	QHBoxLayout * mondaytime5layout = new QHBoxLayout;
	mondaytime5layout->addWidget(time5monday);
	mondaytime5layout->addWidget(starttimemonday5);
	mondaytime5layout->addWidget(endtimemonday5);



	mondaylayout->addLayout(mondaytime1layout);
	mondaylayout->addLayout(mondaytime2layout);
	mondaylayout->addLayout(mondaytime3layout);
	mondaylayout->addLayout(mondaytime4layout);
	mondaylayout->addLayout(mondaytime5layout);

	//周二
	Tuesdaylayout->addWidget(checkTuesday);

	QHBoxLayout *Tuesdaytime1layout = new QHBoxLayout;
	Tuesdaytime1layout->addWidget(time1Tuesday);
	Tuesdaytime1layout->addWidget(starttimeTuesday1);
	Tuesdaytime1layout->addWidget(endtimeTuesday1);

	QHBoxLayout *Tuesdaytime2layout = new QHBoxLayout;
	Tuesdaytime2layout->addWidget(time2Tuesday);
	Tuesdaytime2layout->addWidget(starttimeTuesday2);
	Tuesdaytime2layout->addWidget(endtimeTuesday2);

	QHBoxLayout * Tuesdaytime3layout = new QHBoxLayout;
	Tuesdaytime3layout->addWidget(time3Tuesday);
	Tuesdaytime3layout->addWidget(starttimeTuesday3);
	Tuesdaytime3layout->addWidget(endtimeTuesday3);

	QHBoxLayout * Tuesdaytime4layout = new QHBoxLayout;
	Tuesdaytime4layout->addWidget(time4Tuesday);
	Tuesdaytime4layout->addWidget(starttimeTuesday4);
	Tuesdaytime4layout->addWidget(endtimeTuesday4);

	QHBoxLayout * Tuesdaytime5layout = new QHBoxLayout;
	Tuesdaytime5layout->addWidget(time5Tuesday);
	Tuesdaytime5layout->addWidget(starttimeTuesday5);
	Tuesdaytime5layout->addWidget(endtimeTuesday5);


	Tuesdaylayout->addLayout(Tuesdaytime1layout);
	Tuesdaylayout->addLayout(Tuesdaytime2layout);
	Tuesdaylayout->addLayout(Tuesdaytime4layout);
	Tuesdaylayout->addLayout(Tuesdaytime3layout);
	Tuesdaylayout->addLayout(Tuesdaytime5layout);

	//周三
	Wednesdaylayout->addWidget(checkWednesday);

	QHBoxLayout *Wednesdaytime1layout = new QHBoxLayout;
	Wednesdaytime1layout->addWidget(time1Wednesday);
	Wednesdaytime1layout->addWidget(starttimeWednesday1);
	Wednesdaytime1layout->addWidget(endtimeWednesday1);

	QHBoxLayout *Wednesdaytime2layout = new QHBoxLayout;
	Wednesdaytime2layout->addWidget(time2Wednesday);
	Wednesdaytime2layout->addWidget(starttimeWednesday2);
	Wednesdaytime2layout->addWidget(endtimeWednesday2);

	QHBoxLayout * Wednesdaytime3layout = new QHBoxLayout;
	Wednesdaytime3layout->addWidget(time3Wednesday);
	Wednesdaytime3layout->addWidget(starttimeWednesday3);
	Wednesdaytime3layout->addWidget(endtimeWednesday3);

	QHBoxLayout * Wednesdaytime4layout = new QHBoxLayout;
	Wednesdaytime4layout->addWidget(time4Wednesday);
	Wednesdaytime4layout->addWidget(starttimeWednesday4);
	Wednesdaytime4layout->addWidget(endtimeWednesday4);

	QHBoxLayout * Wednesdaytime5layout = new QHBoxLayout;
	Wednesdaytime5layout->addWidget(time5Wednesday);
	Wednesdaytime5layout->addWidget(starttimeWednesday5);
	Wednesdaytime5layout->addWidget(endtimeWednesday5);



	Wednesdaylayout->addLayout(Wednesdaytime1layout);
	Wednesdaylayout->addLayout(Wednesdaytime2layout);
	Wednesdaylayout->addLayout(Wednesdaytime3layout);
	Wednesdaylayout->addLayout(Wednesdaytime4layout);
	Wednesdaylayout->addLayout(Wednesdaytime5layout);


	//周四
	Thursdaylayout->addWidget(checkThursday);

	QHBoxLayout *Thursdaytime1layout = new QHBoxLayout;
	Thursdaytime1layout->addWidget(time1Thursday);
	Thursdaytime1layout->addWidget(starttimeThursday1);
	Thursdaytime1layout->addWidget(endtimeThursday1);

	QHBoxLayout *Thursdaytime2layout = new QHBoxLayout;
	Thursdaytime2layout->addWidget(time2Thursday);
	Thursdaytime2layout->addWidget(starttimeThursday2);
	Thursdaytime2layout->addWidget(endtimeThursday2);

	QHBoxLayout * Thursdaytime3layout = new QHBoxLayout;
	Thursdaytime3layout->addWidget(time3Thursday);
	Thursdaytime3layout->addWidget(starttimeThursday3);
	Thursdaytime3layout->addWidget(endtimeThursday3);

	QHBoxLayout * Thursdaytime4layout = new QHBoxLayout;
	Thursdaytime4layout->addWidget(time4Thursday);
	Thursdaytime4layout->addWidget(starttimeThursday4);
	Thursdaytime4layout->addWidget(endtimeThursday4);

	QHBoxLayout * Thursdaytime5layout = new QHBoxLayout;
	Thursdaytime5layout->addWidget(time5Thursday);
	Thursdaytime5layout->addWidget(starttimeThursday5);
	Thursdaytime5layout->addWidget(endtimeThursday5);



	Thursdaylayout->addLayout(Thursdaytime1layout);
	Thursdaylayout->addLayout(Thursdaytime2layout);
	Thursdaylayout->addLayout(Thursdaytime3layout);
	Thursdaylayout->addLayout(Thursdaytime4layout);
	Thursdaylayout->addLayout(Thursdaytime5layout);


	//周五
	Fridaylayout->addWidget(checkFriday);

	QHBoxLayout *Fridaytime1layout = new QHBoxLayout;
	Fridaytime1layout->addWidget(time1Friday);
	Fridaytime1layout->addWidget(starttimeFriday1);
	Fridaytime1layout->addWidget(endtimeFriday1);

	QHBoxLayout *Fridaytime2layout = new QHBoxLayout;
	Fridaytime2layout->addWidget(time2Friday);
	Fridaytime2layout->addWidget(starttimeFriday2);
	Fridaytime2layout->addWidget(endtimeFriday2);

	QHBoxLayout * Fridaytime3layout = new QHBoxLayout;
	Fridaytime3layout->addWidget(time3Friday);
	Fridaytime3layout->addWidget(starttimeFriday3);
	Fridaytime3layout->addWidget(endtimeFriday3);

	QHBoxLayout * Fridaytime4layout = new QHBoxLayout;
	Fridaytime4layout->addWidget(time4Friday);
	Fridaytime4layout->addWidget(starttimeFriday4);
	Fridaytime4layout->addWidget(endtimeFriday4);

	QHBoxLayout * Fridaytime5layout = new QHBoxLayout;
	Fridaytime5layout->addWidget(time5Friday);
	Fridaytime5layout->addWidget(starttimeFriday5);
	Fridaytime5layout->addWidget(endtimeFriday5);



	Fridaylayout->addLayout(Fridaytime1layout);
	Fridaylayout->addLayout(Fridaytime2layout);
	Fridaylayout->addLayout(Fridaytime3layout);
	Fridaylayout->addLayout(Fridaytime4layout);
	Fridaylayout->addLayout(Fridaytime5layout);



	//周末
	//Saturdaylayout->addWidget(checkSaturday);
	//Sundaylayout->addWidget(checkSunday);
	


	attendacemanagerlayout->addLayout(mondaylayout);
	attendacemanagerlayout->addLayout(Tuesdaylayout);
	attendacemanagerlayout->addLayout(Wednesdaylayout);
	attendacemanagerlayout->addLayout(Thursdaylayout);
	attendacemanagerlayout->addLayout(Fridaylayout);
	attendacemanagerlayout->addLayout(Saturdaylayout);
	attendacemanagerlayout->addLayout(Sundaylayout);



	QGroupBox* attendancegroup1 = new QGroupBox(tr("attendance time frame"));
	attendancegroup1->setLayout(attendacemanagerlayout);

	
	rightlayout->addWidget(attendancegroup1);
	rightlayout->addStretch(5);
	rightlayout->addWidget(attendancegroup);
	//-----------------




	QHBoxLayout* main_layout = new QHBoxLayout;
	main_layout->addLayout(leftlayout);
	main_layout->addLayout(rightlayout);
	this->setLayout(main_layout);


	updateTree();
}

void AttendanceTabWidget::updateTree()
{
	attendancetree->clear();
	for (auto iter = dataManager->GetAttendance()->begin(); iter != dataManager->GetAttendance()->end(); iter++) 
	{
		auto parent = new QTreeWidgetItem(QStringList() << iter->GetID() + "-" + iter->GetName()+ "-" + iter->GetMID());
		attendancetree->addTopLevelItem(parent);
	}
	//attendancetree->expandAll();

	classtree->clear();
	for (auto iter = dataManager->GetAClass()->begin(); iter != dataManager->GetAClass()->end(); iter++) 
	{
		auto parent = new QTreeWidgetItem(QStringList() << iter->GetID() +"-" + iter->GetName());
		classtree->addTopLevelItem(parent);
	}
	//classtree->expandAll();
}
