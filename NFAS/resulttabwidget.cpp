#include "resulttabwidget.hpp"
#include <QHBoxLayout>
#include <QMessageBox>

#include <QVBoxLayout>
#include <QHeaderView>
#include <QGroupBox>
#include <QFileDialog>
#include <QDateTime>

ResultTabWidget::ResultTabWidget(DataManager *dm, QWidget * parent) : QWidget(parent) 
{
	dataManager = dm;
	setupUi();

	connect(resultadd, SIGNAL(clicked()), this, SLOT(ResultAdd()));
	connect(resultmodify, SIGNAL(clicked()), this, SLOT(ResultAModify()));
	connect(resultdelete, SIGNAL(clicked()), this, SLOT(ResultDelete()));
	connect(attendancefileselect, SIGNAL(clicked()), this, SLOT(AttendanceFileGet()));
	connect(resulttree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(updateTable(QTreeWidgetItem*, int)));
	connect(attendancetable, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(CheckChange(QTableWidgetItem*)));
#ifdef _DEBUG
	connect(debugfile, SIGNAL(clicked()), this, SLOT(GeneTestData()));
#endif
}

ResultTabWidget::~ResultTabWidget()
{
}

void ResultTabWidget::UpdateTab()
{

}

void ResultTabWidget::ResultAModify()//修稿考勤结果数据
{
	if (currenttablefrom == 1)
	{
		auto items = resulttree->selectedItems();

		if (items.size() == 1) 
		{
			QString aid = resulttree->selectedItems()[0]->parent()->text(0).split("-")[0];
			QString rid = resulttree->selectedItems()[0]->text(0);
			int absence = 0;
			int leave = 0;
			QStringList absencsstudent;
			QStringList leavestudent;
			int total = attendancetable->rowCount();
			for (int i = 0; i < total; i++)
			{
				//读取考勤结果
				auto itemid = attendancetable->item(i, 0)->text();
				auto itemname = attendancetable->item(i, 1)->text();
				auto itemaca = attendancetable->item(i, 2)->text();
				auto itemcla = attendancetable->item(i, 3)->text();
				auto itemfigure = attendancetable->item(i, 4)->text();
				auto iteminclass = attendancetable->item(i, 5)->text() == QString(tr("yes")) ? 1 : 0;
				auto itemleaveclass = attendancetable->item(i, 6)->text() == QString(tr("yes")) ? 1 : 0;
				auto itemabclass = attendancetable->item(i, 7)->text() == QString(tr("yes")) ? 1 : 0;

				if (iteminclass == 0) 
				{
					if (itemleaveclass == 1) 
					{
						leave++;
						leavestudent << itemid;
					}
					else
					{
						if (itemabclass == 1) 
						{
							absence++;
							absencsstudent << itemid;
						}
					}
				}
			}
			attendanceresult.SetRID(rid);
			attendanceresult.SetAID(aid);
			attendanceresult.SetANum(total-absence);
			attendanceresult.SetTNum(total);
			attendanceresult.SetLStu(leavestudent.join(","));
			attendanceresult.SetAStu(absencsstudent.join(","));
			attendanceresult.SetUID(dataManager->GetCurrentUser().GetUID());
			if (dataManager->ResultOP(attendanceresult, 1)) {
				QMessageBox::information(0, tr("result modify"), tr("result modify successfully"), QMessageBox::Ok);

				for (auto it = dataManager->GetResult()->begin(); it != dataManager->GetResult()->end(); it++) 
				{
					if (it->GetAID() == attendanceresult.GetAID() && it->GetRID() == attendanceresult.GetRID()) 
					{
						auto abstu = it->GetAStu().split(",");
						auto lestu = it->GetLStu().split(",");
						QStringList allstudent,instu,instudent;
						
						for (auto iter = dataManager->GetAttendance()->begin(); iter != dataManager->GetAttendance()->end(); iter++)
						{
							if (iter->GetID() == it->GetAID()) 
							{
								auto aclass = iter->GetAclass().split(",");
								for (int i = 0; i < aclass.size(); i++) 
								{
									for (auto its = dataManager->GetStudent()->begin(); its != dataManager->GetStudent()->end(); its++) 
									{
										allstudent << its->GetID();
									}
								}
								break;
							}
						}

						for (int i = 0; i < allstudent.size(); i++) {
							if (lestu.indexOf(allstudent[i]) < 0 && abstu.indexOf(allstudent[i]) < 0) 
							{

								instu << allstudent[i];
							}
						}

						for (int i = 0; i < allstudent.size(); i++) {
							if (leavestudent.indexOf(allstudent[i]) < 0 && absencsstudent.indexOf(allstudent[i]) < 0) 
							{
								instudent << allstudent[i];
							}
						}

						for (auto iter = dataManager->GetAttendance()->begin(); iter != dataManager->GetAttendance()->end(); iter++) 
						{
							if (iter->GetID() == it->GetAID()) 
							{
								auto aclass = iter->GetAclass().split(",");
								for (int i = 0; i < aclass.size(); i++) 
								{

									for (auto its = dataManager->GetStudent()->begin(); its != dataManager->GetStudent()->end(); its++) 
									{
										if (its->GetAclass() == aclass[i]) 
										{
											if (absencsstudent.indexOf(its->GetID())>=0) 
											{
												if (abstu.indexOf(its->GetID()) < 0) 
												{
													its->AddAbtime();
													if (lestu.indexOf(its->GetID()) > 0) 
													{
														its->MinusLetime();
													}
													else {
														its->MinusIntime();
													}
												}
											}
											else if (leavestudent.indexOf(its->GetID()) >= 0) 
											{
												if (lestu.indexOf(its->GetID()) < 0) 
												{
													its->AddLetime();
													if (abstu.indexOf(its->GetID()) > 0) 
													{
														its->MinusAbtime();
													}
													else 
													{
														its->MinusIntime();
													}
												}
											}
											else if(instudent.indexOf(its->GetID()) >= 0)
											{
												if (instu.indexOf(its->GetID()) < 0) 
												{
													its->AddIntime();
													if (abstu.indexOf(its->GetID()) > 0) 
													{
														its->MinusAbtime();
													}
													else {
														its->MinusLetime();
													}
												}
											}
											dataManager->StudentOP(Student(its->GetID(), "", 0, "", "", "", its->GetIntimes(), its->GetLetimes(), its->GetAbtimes(), its->GetTotimes()), 6);
										}
									}
								}
								break;
							}
						}
						break;
					}
				}

				dataManager->updateResult(dataManager->GetCurrentUser());
			}
			else
			{
				QMessageBox::information(0, tr("result modify"), tr("result modify failed"), QMessageBox::Ok);
			}
		}
		else 
		{
			QMessageBox::information(0, tr("result modify"), tr("please select result first"), QMessageBox::Ok);
		}
	}
	else
	{
		QMessageBox::information(0, tr("result modify"), tr("please select result first"), QMessageBox::Ok);
	}
}

void ResultTabWidget::ResultDelete()
{
	if (currenttablefrom == 1)
	{
		auto items = resulttree->selectedItems();
		auto parent = items[0]->parent();
		if (parent == nullptr) 
		{
			QMessageBox::information(0, tr("result delete"), tr("please select result first"), QMessageBox::Ok);
		}
		else 
		{
			if (items.size() == 1) 
			{
				QString aid = parent->text(0).split("-")[0];
				QString rid = items[0]->text(0);

				attendanceresult.SetAID(aid);
				attendanceresult.SetRID(rid);

				if (dataManager->ResultOP(attendanceresult, 2))
				{
					for (auto it = dataManager->GetResult()->begin(); it != dataManager->GetResult()->end(); it++) 
					{
						auto abstu = it->GetAStu().split(",");
						auto lestu = it->GetLStu().split(",");
						if (it->GetAID() == aid&&it->GetRID() == rid)
						{
							for (auto iter = dataManager->GetAttendance()->begin(); iter != dataManager->GetAttendance()->end(); iter++)
							{
								if (iter->GetID() == it->GetAID()) 
								{
									auto aclass = iter->GetAclass().split(",");
									for (int i = 0; i < aclass.size(); i++)
									{
										for (auto its = dataManager->GetStudent()->begin(); its != dataManager->GetStudent()->end(); its++)
										{
											if (its->GetAclass() == aclass[i]) 
											{
												its->MinusTotime();
												if (abstu.indexOf(its->GetID()) >= 0) 
												{
													its->MinusAbtime();
													dataManager->StudentOP(Student(its->GetID(), "", 0, "", "", "", 0, 0, its->GetAbtimes(),its->GetTotimes()), 5);
													
												}
												else if (lestu.indexOf(its->GetID()) >= 0) 
												{
													its->MinusLetime();
													dataManager->StudentOP(Student(its->GetID(), "", 0, "", "", "", 0, its->GetLetimes(), 0, its->GetTotimes()), 4);
												}
												else 
												{
													its->MinusIntime();
													dataManager->StudentOP(Student(its->GetID(), "", 0, "", "", "", its->GetIntimes(), 0, 0, its->GetTotimes()), 3);
												}
											}
										}
									}
									break;
								}
							}
							break;
						}
					}
					QMessageBox::information(0, tr("result delete"), tr("result delete successfully"), QMessageBox::Ok);
					dataManager->updateResult(dataManager->GetCurrentUser());
					updateTree();
				}
				else
				{
					QMessageBox::information(0, tr("result delete"), tr("result delete failed"), QMessageBox::Ok);
				}
			}
			else 
			{
				QMessageBox::information(0, tr("result delete"), tr("please select result first"), QMessageBox::Ok);
			}
		}
		
	}
	else if (currenttablefrom == 2)
	{
		auto items = resulttree->selectedItems();
		auto parent = items[0]->parent();
		if (parent == nullptr) {
			QString aid = items[0]->text(0).split("-")[0];
			
			attendanceresult.SetAID(aid);
			attendanceresult.SetRID("");

			if (dataManager->ResultOP(attendanceresult, 2)) 
			{
				QMessageBox::information(0, tr("result delete"), tr("result delete successfully"), QMessageBox::Ok);
				for (auto it = dataManager->GetResult()->begin(); it != dataManager->GetResult()->end(); it++) 
				{
					auto abstu = it->GetAStu().split(",");
					auto lestu = it->GetLStu().split(",");
					if (it->GetAID() == aid) {
						for (auto iter = dataManager->GetAttendance()->begin(); iter != dataManager->GetAttendance()->end(); iter++) 
						{
							if (iter->GetID() == it->GetAID()) 
							{
								auto aclass = iter->GetAclass().split(",");
								for (int i = 0; i < aclass.size(); i++) 
								{
									for (auto its = dataManager->GetStudent()->begin(); its != dataManager->GetStudent()->end(); its++)
									{
										if (its->GetAclass() == aclass[i]) 
										{
											its->MinusTotime();
											if (abstu.indexOf(its->GetID()) >= 0)
											{
												its->MinusAbtime();
												dataManager->StudentOP(Student(its->GetID(), "", 0, "", "", "", 0, 0, its->GetAbtimes(),its->GetTotimes()), 5);

											}
											else if (lestu.indexOf(its->GetID()) >= 0) 
											{
												its->MinusLetime();
												dataManager->StudentOP(Student(its->GetID(), "", 0, "", "", "", 0, its->GetLetimes(), 0, its->GetTotimes()), 4);
											}
											else 
											{
												its->MinusIntime();
												dataManager->StudentOP(Student(its->GetID(), "", 0, "", "", "", its->GetIntimes(), 0, 0, its->GetTotimes()), 3);
											}
										}
									}
								}
								break;
							}
						}
					}
				}
				dataManager->updateResult(dataManager->GetCurrentUser());
				updateTree();
			}
			else
			{
				QMessageBox::information(0, tr("result delete"), tr("result delete failed"), QMessageBox::Ok);
			}
		}
		else 
		{
			QMessageBox::information(0, tr("result delete"), tr("please select result first"), QMessageBox::Ok);
		}
	}
	else 
	{
		QMessageBox::information(0, tr("result delete"), tr("please select result first"), QMessageBox::Ok);
	}
	attendancetable->clear();
}

void ResultTabWidget::updateTable(QTreeWidgetItem * item, int col)
{
	auto parent = item->parent();
	if (parent == nullptr)
	{
		currenttablefrom = 2;
		attendancetable->clear();
		attendancetable->setRowCount(1);
		attendancetable->setColumnCount(4);
		QStringList header;
		header << tr("attendance id") <<tr("attendance name")<< tr("miss number") << tr("on rate") ;
		attendancetable->setHorizontalHeaderLabels(header);
		auto id = item->text(0).split("-")[0];
		QString aname = "";
		int missnumber = 0;
		int totalnumber = 0;
		for (auto it = dataManager->GetAttendance()->begin(); it != dataManager->GetAttendance()->end(); it++) 
		{
			if (it->GetID() == id)
			{
				aname = it->GetName();
				break;
			}
		}
		for (auto it = dataManager->GetResult()->begin(); it != dataManager->GetResult()->end(); it++)
		{
			if (it->GetAID() == id) 
			{
				int ls=0,as=0;
				if (it->GetLStu().length() > 0) 
				{
					ls = it->GetLStu().split(",").size();
				}
				if (it->GetAStu().length() > 0) 
				{
					as = it->GetAStu().split(",").size();
				}
				missnumber += (as+ls);
				totalnumber += it->GetTNum();
			}
		}
		attendancetable->setItem(0, 0, new QTableWidgetItem(id));
		attendancetable->setItem(0, 1, new QTableWidgetItem(aname));
		attendancetable->setItem(0, 2, new QTableWidgetItem(QString::number(missnumber)));
		attendancetable->setItem(0, 3, new QTableWidgetItem(QString::number((1.0-((double)missnumber)/(double)totalnumber)*100.0,'f',2)+"%"));

		for (int i = 0; i < attendancetable->rowCount(); i++)
		{
			attendancetable->item(i, 0)->setFlags(attendancetable->item(i, 0)->flags() & (~Qt::ItemIsEditable));
			attendancetable->item(i, 1)->setFlags(attendancetable->item(i, 1)->flags() & (~Qt::ItemIsEditable));
			attendancetable->item(i, 2)->setFlags(attendancetable->item(i, 2)->flags() & (~Qt::ItemIsEditable));
			attendancetable->item(i, 3)->setFlags(attendancetable->item(i, 3)->flags() & (~Qt::ItemIsEditable));
		}
	}
	else 
	{
		currenttablefrom = 1;
		attendancetable->clear();
		attendancetable->setRowCount(0);
		attendancetable->setColumnCount(8);
		QStringList header;
		header << tr("student id") << tr("student name") << tr("student academic") << tr("student class")
			<< tr("student figure") << tr("in class") << tr("leave class") << tr("absencs class");
		auto aid = parent->text(0).split("-")[0];
		attendancetable->setHorizontalHeaderLabels(header);
		auto rid = item->text(0);
		auto total = 0;
		auto leave = 0;
		auto abnumber = 0;
		for (auto it = dataManager->GetResult()->begin(); it != dataManager->GetResult()->end(); it++)
		{
			if (it->GetRID() == rid) 
			{
				auto lestudent = it->GetLStu().split(",");
				auto abstudent = it->GetAStu().split(",");
				for (auto iter = dataManager->GetAttendance()->begin(); iter != dataManager->GetAttendance()->end(); iter++)
				{
					if (iter->GetID() == it->GetAID()) 
					{

						auto aclass = iter->GetAclass().split(",");
						for (int i = 0; i < aclass.size(); i++) 
						{
							for (auto its = dataManager->GetStudent()->begin(); its != dataManager->GetStudent()->end(); its++)
							{
								if (its->GetAclass() == aclass.at(i)) 
								{
									total++;
									auto rowcount = attendancetable->rowCount();
									attendancetable->insertRow(rowcount);
									attendancetable->setItem(rowcount, 0, new QTableWidgetItem(its->GetID()));
									attendancetable->setItem(rowcount, 1, new QTableWidgetItem(its->GetName()));
									attendancetable->setItem(rowcount, 2, new QTableWidgetItem(its->GetAcademic()));
									attendancetable->setItem(rowcount, 3, new QTableWidgetItem(its->GetAclass()));
									attendancetable->setItem(rowcount, 4, new QTableWidgetItem(its->GetFigure()));
									QString inbox = QString(tr("no"));
									QString lebox = QString(tr("no"));
									QString abbox = QString(tr("no"));
									if (lestudent.indexOf(its->GetID()) >= 0) 
									{
										leave++;
										lebox = QString(tr("yes"));
										/*its->AddLetime();
										dataManager->StudentOP(Student(its->GetID(), "", 0, "", "", "", 0, its->GetLetimes(), 0), 4);*/
									}
									else if (abstudent.indexOf(its->GetID()) >= 0) 
									{
										abnumber++;
										abbox = QString(tr("yes"));
										/*its->AddAbtime();
										dataManager->StudentOP(Student(its->GetID(), "", 0, "", "", "", 0, 0, its->GetAbtimes()), 5);*/
									}
									else
									{
										inbox = QString(tr("yes"));
										/*its->AddIntime();
										dataManager->StudentOP(Student(its->GetID(), "", 0, "", "", "", its->GetIntimes(), 0, 0), 3);*/
									}
									attendancetable->setItem(rowcount, 5, new QTableWidgetItem(inbox));
									attendancetable->setItem(rowcount, 6, new QTableWidgetItem(lebox));
									attendancetable->setItem(rowcount, 7, new QTableWidgetItem(abbox));
								}
							}
						}
						break;
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
			attendancetable->item(i, 6)->setFlags(attendancetable->item(i, 6)->flags() & (~Qt::ItemIsEditable));
			attendancetable->item(i, 7)->setFlags(attendancetable->item(i, 7)->flags() & (~Qt::ItemIsEditable));
		}

		totalnumber->setText(QString::number(total));
		leavenumber->setText(QString::number(leave));
		absencenumber->setText(QString::number(abnumber));
	}
}

void ResultTabWidget::AttendanceFileGet()
{
	currenttablefrom = 0;
	attendancetable->clear();
	attendancetable->setRowCount(0);
	attendancetable->setColumnCount(8);
	QStringList header;
	header << tr("student id") << tr("student name") << tr("student academic") << tr("student class")
		<< tr("student figure") << tr("in class") << tr("leave class") << tr("absencs class");
	attendancetable->setHorizontalHeaderLabels(header);

	QString filename = QFileDialog::getOpenFileName(NULL, tr("select attendance file"), ".", "Excel Files(*.xlsx)");
	attendencefile->setText(filename);
	QXlsx::Document xlsx(filename);

	if (xlsx.read(1, 1).toString() != tr("student id") || xlsx.read(1, 2).toString() != tr("student name")
		|| xlsx.read(1, 3).toString() != tr("student academic") || xlsx.read(1, 4).toString() != tr("student class")
		|| xlsx.read(1, 5).toString() != tr("student figure") || xlsx.read(1, 6).toString() != tr("in class")
		|| xlsx.read(1, 7).toString() != tr("leave class") || xlsx.read(1, 8).toString() != tr("absencs class")) 
	{
		QMessageBox::information(0, tr("attendance file select"), tr("please select right file"), QMessageBox::Ok);
		return;
	}

	int total = 0;
	int absence = 0;
	int leave = 0;
	QStringList absencsstudent;
	QStringList leavestudent;
	for (int row = 2;; row++) 
	{
		QString stuid = xlsx.read(row, 1).toString();
		if (stuid == "")	break;
		total++;
		QString stuname = xlsx.read(row, 2).toString();
		QString stuaca = xlsx.read(row, 3).toString();
		QString stucla = xlsx.read(row, 4).toString();
		QString stufigure = xlsx.read(row, 5).toString();
		int stuin = xlsx.read(row, 6).toInt();
		int stuleave = xlsx.read(row, 7).toInt();
		int stuabsencs = xlsx.read(row, 8).toInt();

		auto rowcount = attendancetable->rowCount();
		attendancetable->insertRow(rowcount);
		attendancetable->setItem(rowcount, 0, new QTableWidgetItem(stuid));
		attendancetable->setItem(rowcount, 1, new QTableWidgetItem(stuname));
		attendancetable->setItem(rowcount, 2, new QTableWidgetItem(stuaca));
		attendancetable->setItem(rowcount, 3, new QTableWidgetItem(stucla));
		attendancetable->setItem(rowcount, 4, new QTableWidgetItem(stufigure));

		QString inbox=QString(tr("no"));
		QString lebox = QString(tr("no"));
		QString abbox = QString(tr("no"));

		if (stuin == 1) 
		{
			inbox = QString(tr("yes"));
		}
		else if (stuleave == 1) 
		{
			lebox = QString(tr("yes"));
		}
		else if (stuabsencs == 1) 
		{
			abbox = QString(tr("yes"));
		}

		attendancetable->setItem(rowcount, 5, new QTableWidgetItem(inbox));
		attendancetable->setItem(rowcount, 6, new QTableWidgetItem(lebox));
		attendancetable->setItem(rowcount, 7, new QTableWidgetItem(abbox));

		if (stuin == 0) 
		{
			if (stuleave == 1)
			{
				leave++;
				leavestudent << stuid;
			}
			else {
				if (stuabsencs == 1) 
				{
					absence++;
					absencsstudent << stuid;
				}
			}
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
		attendancetable->item(i, 6)->setFlags(attendancetable->item(i, 6)->flags() & (~Qt::ItemIsEditable));
		attendancetable->item(i, 7)->setFlags(attendancetable->item(i, 7)->flags() & (~Qt::ItemIsEditable));
	}

	QString attenfileid = filename.split("/").last().split("-")[0];
	attendanceresult.SetAID(attenfileid);
	QDateTime datetime;
	auto now = datetime.currentDateTime();
	attendanceresult.SetRID(now.toString("yyyyMMddHHmmss"));

	totalnumber->setText(QString::number(total));
	leavenumber->setText(QString::number(leave));
	absencenumber->setText(QString::number(absence));
}

void ResultTabWidget::CheckChange(QTableWidgetItem * item)
{
	if (attendancetable->columnCount() == 8) 
	{
		auto col = attendancetable->currentColumn();
		if (col >= 5 && col <= 7) {
			auto row = attendancetable->currentRow();
			if (col == 5) 
			{
				if (attendancetable->item(row, 5)->text() == QString(tr("no"))) 
				{
					attendancetable->item(row, 5)->setText(QString(tr("yes")));
					attendancetable->item(row, 6)->setText(QString(tr("no")));
					attendancetable->item(row, 7)->setText(QString(tr("no")));
				}
			}
			else if (col == 6)
			{
				if (attendancetable->item(row, 6)->text() == QString(tr("no"))) 
				{
					attendancetable->item(row, 6)->setText(QString(tr("yes")));
					attendancetable->item(row, 5)->setText(QString(tr("no")));
					attendancetable->item(row, 7)->setText(QString(tr("no")));
				}
			}
			else if (col == 7)
			{
				if (attendancetable->item(row, 7)->text() == QString(tr("no"))) 
				{

					attendancetable->item(row, 7)->setText(QString(tr("yes")));
					attendancetable->item(row, 5)->setText(QString(tr("no")));
					attendancetable->item(row, 6)->setText(QString(tr("no")));
				}
			}
		}
		int total = attendancetable->rowCount();
		int absence = 0;
		int leave = 0;

		for (int i = 0; i < total; i++) {
			if (attendancetable->item(i, 6)->text() == QString(tr("yes"))) 
			{
				leave++;
			}
			else if (attendancetable->item(i, 7)->text() == QString(tr("yes")))
			{
				absence++;
			}
		}

		totalnumber->setText(QString::number(total));
		leavenumber->setText(QString::number(leave));
		absencenumber->setText(QString::number(absence));
	}
}

void ResultTabWidget::GeneTestData()
{
#ifdef _DEBUG//测试考勤结果生成
	auto aid = attendance->currentText().split("-")[0];
	for (auto it = dataManager->GetAttendance()->begin(); it != dataManager->GetAttendance()->end(); it++)
	{
		if (it->GetID() == aid) 
		{
			
			QString filename = attendance->currentText() + ".xlsx";
			QFile file(filename);
			QXlsx::Document xlsx;
			QXlsx::Format format;
			format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
			format.setVerticalAlignment(QXlsx::Format::AlignVCenter);
			xlsx.write(1, 1, tr("student id"), format);
			xlsx.write(1, 2, tr("student name"), format);
			xlsx.write(1, 3, tr("student academic"), format);
			xlsx.write(1, 4, tr("student class"), format);
			xlsx.write(1, 5, tr("student figure"), format);
			xlsx.write(1, 6, tr("in class"), format);
			xlsx.write(1, 7, tr("leave class"), format);
			xlsx.write(1, 8, tr("absencs class"), format);
			xlsx.setColumnWidth(5, 40);
			auto aclass = it->GetAclass().split(",");
			int index = 2;
			for (int i = 0; i < aclass.size(); i++) 
			{
				auto classid = aclass.at(i);
				for (auto iter = dataManager->GetStudent()->begin(); iter != dataManager->GetStudent()->end(); iter++)
				{
					if (classid == iter->GetAclass()) 
					{
						xlsx.write(index, 1,iter->GetID(), format);
						xlsx.write(index, 2, iter->GetName(), format);
						xlsx.write(index, 3, iter->GetAcademic(), format);
						xlsx.write(index, 4, iter->GetAclass(), format);
						xlsx.write(index, 5, iter->GetFigure(), format);
						xlsx.write(index, 6, 0, format);
						xlsx.write(index, 7, 0, format);
						xlsx.write(index++, 8, 0, format);
						qDebug() <<tr("student information output:") <<iter->GetID() << "\t" << iter->GetName() << "\n";
					}
				}
			}
			xlsx.saveAs(filename);
			break;
		}
	}
#endif
}

void ResultTabWidget::ResultAdd()
{
	if (currenttablefrom == 0) 
	{
		int absence = 0;
		int leave = 0;
		QStringList absencsstudent;
		QStringList leavestudent;
		int total = attendancetable->rowCount();
		for (int i = 0; i < total; i++)
		{
			auto itemid = attendancetable->item(i, 0)->text();
			auto itemname = attendancetable->item(i, 1)->text();
			auto itemaca = attendancetable->item(i, 2)->text();
			auto itemcla = attendancetable->item(i, 3)->text();
			auto itemfigure = attendancetable->item(i, 4)->text();
			auto iteminclass = attendancetable->item(i, 5)->text() == QString(tr("yes")) ? 1 : 0;
			auto itemleaveclass = attendancetable->item(i, 6)->text() == QString(tr("yes")) ? 1 : 0;
			auto itemabclass = attendancetable->item(i, 7)->text() == QString(tr("yes")) ? 1 : 0;

			if (iteminclass == 0) 
			{
				if (itemleaveclass == 1) 
				{
					leave++;
					leavestudent << itemid;
				}
				else 
				{
					if (itemabclass == 1)
					{
						absence++;
						absencsstudent << itemid;
					}
				}
			}
		}
		attendanceresult.SetANum(total - absence);
		attendanceresult.SetTNum(total);
		attendanceresult.SetLStu(leavestudent.join(","));
		attendanceresult.SetAStu(absencsstudent.join(","));
		attendanceresult.SetUID(dataManager->GetCurrentUser().GetUID());
		if (dataManager->ResultOP(attendanceresult, 0)) 
		{
			QMessageBox::information(0, tr("result add"), tr("result add successfully"), QMessageBox::Ok);
			dataManager->updateResult(dataManager->GetCurrentUser());
			for (auto it = dataManager->GetResult()->begin(); it != dataManager->GetResult()->end(); it++)
			{
				auto abstu = it->GetAStu().split(",");
				auto lestu = it->GetLStu().split(",");
				if (it->GetAID() == attendanceresult.GetAID()&&it->GetRID() == attendanceresult.GetRID())
				{
					for (auto iter = dataManager->GetAttendance()->begin(); iter != dataManager->GetAttendance()->end(); iter++) 
					{
						if (iter->GetID() == it->GetAID()) 
						{
							auto aclass = iter->GetAclass().split(",");
							for (int i = 0; i < aclass.size(); i++) 
							{
								for (auto its = dataManager->GetStudent()->begin(); its != dataManager->GetStudent()->end(); its++) 
								{
									if (its->GetAclass() == aclass[i]) 
									{
										its->AddTotime();
										if (abstu.indexOf(its->GetID()) >= 0) 
										{
											its->AddAbtime();
											dataManager->StudentOP(Student(its->GetID(), "", 0, "", "", "", 0, 0, its->GetAbtimes(),its->GetTotimes()), 5);

										}
										else if (lestu.indexOf(its->GetID()) >= 0) 
										{
											its->AddLetime();
											dataManager->StudentOP(Student(its->GetID(), "", 0, "", "", "", 0, its->GetLetimes(), 0, its->GetTotimes()), 4);
										}
										else 
										{
											its->AddIntime();
											dataManager->StudentOP(Student(its->GetID(), "", 0, "", "", "", its->GetIntimes(), 0, 0, its->GetTotimes()), 3);
										}
									}
								}
							}
							break;
						}
					}
					break;
				}
			}
			updateTree();
		}
		else
		{
			QMessageBox::information(0, tr("result add"), tr("result add failed"), QMessageBox::Ok);
		}
	}
	else
	{
		QMessageBox::information(0, tr("result add"), tr("please select attendance result file first"), QMessageBox::Ok);
	}
}

void ResultTabWidget::setupUi()
{
	resulttree = new QTreeWidget;
	resulttree->setHeaderLabel(tr("result information"));
	resulttree->setMaximumWidth(150);
	QLabel* labelattendancefile = new QLabel(tr("attendance file select"));
	attendencefile = new QLineEdit;
	attendancefileselect = new QPushButton(tr("select attendance file"));
	attendancefileselect->setMinimumWidth(120);
	attendancetable = new QTableWidget;
	attendancetable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	resultadd = new QPushButton(tr("result add"));
	
	resultmodify = new QPushButton(tr("result modify"));
	resultdelete = new QPushButton(tr("result delete"));
	QHBoxLayout* filelayout = new QHBoxLayout;
	filelayout->addWidget(labelattendancefile);
	filelayout->addWidget(attendencefile);
	filelayout->addWidget(attendancefileselect);
	QVBoxLayout* resultinfo = new QVBoxLayout;
	QHBoxLayout* resultinfolayout = new QHBoxLayout;
	QLabel* labeltotalnumber = new QLabel(tr("total number"));
	totalnumber = new QLabel;
	QLabel* labelleavenumber = new QLabel(tr("leave number"));
	leavenumber = new QLabel;
	QLabel* labelabsencsnumber = new QLabel(tr("absence number"));
	absencenumber = new QLabel;
	resultinfolayout->addWidget(labeltotalnumber);
	resultinfolayout->addWidget(totalnumber);
	resultinfolayout->addStretch(1);
	resultinfolayout->addWidget(labelleavenumber);
	resultinfolayout->addWidget(leavenumber);
	resultinfolayout->addStretch(1);
	resultinfolayout->addWidget(labelabsencsnumber);
	resultinfolayout->addWidget(absencenumber);
	resultinfolayout->addStretch(10);
	resultinfo->addLayout(resultinfolayout);
	QHBoxLayout* attendanceop = new QHBoxLayout;
	attendanceop->addStretch(1);
	attendanceop->addWidget(resultadd);
	attendanceop->addWidget(resultmodify);
	attendanceop->addWidget(resultdelete);
	resultinfo->addLayout(attendanceop);
	QGroupBox* groupop = new QGroupBox(tr("result administrator"));
	groupop->setLayout(resultinfo);
	QHBoxLayout* main_layout = new QHBoxLayout;
	QVBoxLayout* rightlayout = new QVBoxLayout;
	rightlayout->addLayout(filelayout);
	rightlayout->addWidget(attendancetable);
	rightlayout->addWidget(groupop);
	main_layout->addWidget(resulttree);
	main_layout->addLayout(rightlayout);

#ifdef _DEBUG//生成考勤测试文件
	debugfile=new QPushButton(tr("generate test file"));
	QHBoxLayout* debuglayout = new QHBoxLayout;
	attendance = new QComboBox;
	for (auto it=dataManager->GetAttendance()->begin();it!=dataManager->GetAttendance()->end();it++)
	{
		attendance->addItem(it->GetID() + "-" + it->GetName());
	}
	debuglayout->addWidget(attendance);
	debuglayout->addStretch(1);
	debuglayout->addWidget(debugfile);
	rightlayout->addLayout(debuglayout);
#endif

	this->setLayout(main_layout);

	updateTree();
}

void ResultTabWidget::updateTree()
{
	resulttree->clear();
	QStringList current;
	for (auto it = dataManager->GetResult()->begin(); it != dataManager->GetResult()->end(); it++) 
	{
		if (current.indexOf(it->GetAID()) < 0) 
		{
			current << it->GetAID();
			auto parent = new QTreeWidgetItem(QStringList() << it->GetAID() + "-" + it->GetUID());
			for (auto iter = dataManager->GetResult()->begin(); iter != dataManager->GetResult()->end(); iter++)
			{
				if (iter->GetAID() == it->GetAID()) 
				{
					parent->addChild(new QTreeWidgetItem(QStringList() << iter->GetRID()));
				}
			}
			resulttree->addTopLevelItem(parent);
		}
	}
	resulttree->expandAll();
}
