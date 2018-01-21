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
	attendanceop->addStretch(2);
	attendanceop->addWidget(attendanceadd);
	attendanceop->addWidget(attendancemodify);
	attendanceop->addWidget(attendancedelete);
	attendacelayout->addLayout(attendanceop);
	attendancegroup->setLayout(attendacelayout);
	rightlayout->addWidget(attendancegroup);

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
