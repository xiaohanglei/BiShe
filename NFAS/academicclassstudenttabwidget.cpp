#include "academicclassstudenttabwidget.hpp"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QHeaderView>
#include <QComboBox>

AcademicClassStudentTabWidget::AcademicClassStudentTabWidget(DataManager *dm, QWidget * parent) : QWidget(parent)
{
	dataManager = dm;
	setupUi();
	connect(academicadd, SIGNAL(clicked()), this, SLOT(AcademicAdd()));//添加学院
	//connect(academicmodify, SIGNAL(clicked()), this, SLOT(AcademicModify()));//修改
	connect(academicdelete, SIGNAL(clicked()), this, SLOT(AcademicDelete()));//删除学院

	//班级
	connect(classadd, SIGNAL(clicked()), this, SLOT(ClassAdd()));
	connect(classmodify, SIGNAL(clicked()), this, SLOT(ClassModify()));
	connect(classdelete, SIGNAL(clicked()), this, SLOT(ClassDelete()));

	//学生信息
	connect(studentadd, SIGNAL(clicked()), this, SLOT(StudentAdd()));
	connect(studentmodify, SIGNAL(clicked()), this, SLOT(StudentModify()));
	connect(studentdelete, SIGNAL(clicked()), this, SLOT(StudentDelete()));

	//当点击学院信息列表框时触发该槽函数
	connect(studentacademic, SIGNAL(currentIndexChanged(int)), this, SLOT(StudentAcademicClassChange(int)));

	//
	connect(academictree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(updateTable(QTreeWidgetItem*,int)));
}

AcademicClassStudentTabWidget::~AcademicClassStudentTabWidget() 
{
	
}

void AcademicClassStudentTabWidget::UpdateTab()
{

}

void AcademicClassStudentTabWidget::AcademicAdd()
{
	QString acaid = academicid->text().trimmed();
	QString acaname = academicname->text().trimmed();
	if (acaid.length() != 4)
	{
		QMessageBox::information(0, tr("academic add"), tr("length of academic id must be 4"), QMessageBox::Ok);
		return;
	}
	if (!(acaname.length() > 0)) 
	{
		QMessageBox::information(0, tr("academic add"), tr("academic name is required"), QMessageBox::Ok);
		return;
	}
	Academic academic(acaid, acaname);
	if (dataManager->AcademicOP(academic, 0))//讲该学院插入数据库中
	{
		QMessageBox::information(0, tr("academic add"), tr("academic add successfully"), QMessageBox::Ok);
		academicid->setText("");
		academicname->setText("");
		dataManager->updateAcademic();//更新数据库中的数据
		updateComboBox();//更新列表框
		updateTree();
	}
	else 
	{
		QMessageBox::information(0, tr("academic add"), tr("academic add failed"), QMessageBox::Ok);
	}
}

//void AcademicClassStudentTabWidget::AcademicModify()
//{
//	
//}

void AcademicClassStudentTabWidget::AcademicDelete()
{
	auto s=academictree->selectedItems()[0];
	auto p=s->parent();
	if (p == nullptr)
	{
		if (s->childCount() > 0) //确认删除
		{
			if (QMessageBox::information(0, tr("academic delete"), tr("class is not 0,are you sure to delete?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
			{
				return;
			}
		}
		for (int i = 0; i < s->childCount(); i++)
		{
			dataManager->AclassOP(Aclass(s->child(i)->text(0).split("-")[0], "", ""), 2);
			for (auto it = dataManager->GetStudent()->begin(); it != dataManager->GetStudent()->end(); it++) 
			{
				if (it->GetAclass() == s->child(i)->text(0).split("-")[0])
				{
					dataManager->StudentOP(Student(it->GetID(), "", 0, "", "", ""), 2);
				}
			}
		}
		dataManager->updateStudent();
		dataManager->updateClass();
		QString acaid = s->text(0).split("-")[0];
		if (dataManager->AcademicOP(Academic(acaid,""), 1)) 
		{
			QMessageBox::information(0, tr("academic delete"), tr("academic delete successfully"), QMessageBox::Ok);
			dataManager->updateAcademic();
			updateComboBox();
			updateTree();
		}
		else 
		{
			QMessageBox::information(0, tr("academic delete"), tr("academic delete failed"), QMessageBox::Ok);
		}
	}
	else
	{
		QMessageBox::information(0, tr("academic delete"), tr("please select academic first"), QMessageBox::Ok);
	}

}

void AcademicClassStudentTabWidget::ClassAdd()
{
	QString claid = classid->text().trimmed();
	QString claname = classname->text().trimmed();
	QString claaca = classacademic->currentText().split("-")[0];
	if (claid.length() != 4) 
	{
		QMessageBox::information(0, tr("class add"), tr("length of class id must be 4"), QMessageBox::Ok);
		return;
	}
	if (!(claname.length() > 0))
	{
		QMessageBox::information(0, tr("class add"), tr("class name is required"), QMessageBox::Ok);
		return;
	}
	Aclass clas(claid, claname, claaca);
	if (dataManager->AclassOP(clas, 0)) 
	{
		QMessageBox::information(0, tr("class add"), tr("class add successfully"), QMessageBox::Ok);
		classid->setText("");
		classname->setText("");
		dataManager->updateClass();
		updateTree();
		updateComboBox();
	}
	else 
	{
		QMessageBox::information(0, tr("class add"), tr("class add failed"), QMessageBox::Ok);
	}
}

void AcademicClassStudentTabWidget::ClassModify()
{
	auto items = academictree->selectedItems();
	if (items.length() != 1)
	{
		QMessageBox::information(0, tr("class modify"), tr("please select academic first"), QMessageBox::Ok);
		return;
	}
	auto item = items[0];
	if (item->parent() == nullptr)
	{
		bool success = false;
		for (int i = 0; i < academicclasstable->rowCount(); i++) 
		{
			QString claid = academicclasstable->item(i, 0)->text();
			QString claname= academicclasstable->item(i, 1)->text();
			auto item = (QComboBox*)academicclasstable->cellWidget(i, 2);
			QString claaca = item->currentText();
			Aclass clas(claid, claname, claaca);
			success = dataManager->AclassOP(clas, 1);
		}
		if (success)
		{
			QMessageBox::information(0, tr("class modify"), tr("class modify successfully"), QMessageBox::Ok);
			classid->setText("");
			classname->setText("");
			dataManager->updateClass();
			updateTree();
			updateComboBox();
		}
		else 
		{
			QMessageBox::information(0, tr("class modify"), tr("class modify failed"), QMessageBox::Ok);
		}
	}
	else
	{
		QMessageBox::information(0, tr("class modify"), tr("please select academic first"), QMessageBox::Ok);
	}
}

void AcademicClassStudentTabWidget::ClassDelete()
{
	auto s = academictree->selectedItems()[0];
	auto p = s->parent();
	if (p != nullptr) 
	{
		QString classid = s->text(0).split("-")[0];
		for (auto it = dataManager->GetStudent()->begin(); it != dataManager->GetStudent()->end(); it++) 
		{
			if (it->GetAclass() == classid) 
			{
				dataManager->StudentOP(Student(it->GetID(), "",0,"","",""), 2);
			}
		}
		dataManager->updateStudent();

		if (dataManager->AclassOP(Aclass(classid, "",""), 2)) 
		{
			QMessageBox::information(0, tr("class delete"), tr("class delete successfully"), QMessageBox::Ok);
			dataManager->updateClass();
			updateTree();
			updateComboBox();
		}
		else 
		{
			QMessageBox::information(0, tr("class delete"), tr("class delete failed"), QMessageBox::Ok);
		}
	}
	else
	{
		QMessageBox::information(0, tr("class delete"), tr("please select class first"), QMessageBox::Ok);
	}
}

void AcademicClassStudentTabWidget::StudentAdd()
{
	QString stuid = studentid->text().trimmed();
	QString stuname = studentname->text().trimmed();
	int stusex = studentsex->currentIndex();
	QString stuaca = studentacademic->currentText().split("-")[0];
	QString stucla = studentclass->currentText().split("-")[0];
	if (stuid.length() != 8) 
	{
		QMessageBox::information(0, tr("student add"), tr("length of student id must be 8"), QMessageBox::Ok);
		return;
	}
	if (!(stuname.length() > 0)) 
	{
		QMessageBox::information(0, tr("student add"), tr("student name is required"), QMessageBox::Ok);
		return;
	}
	Student student(stuid, stuname,stusex,stuaca,stucla,GeneFigureInfo(stuid,stuname));
	if (dataManager->StudentOP(student, 0))
	{
		QMessageBox::information(0, tr("student add"), tr("student add successfully"), QMessageBox::Ok);
		studentid->setText("");
		studentname->setText("");
		dataManager->updateStudent();
	}
	else
	{
		QMessageBox::information(0, tr("student add"), tr("student add failed"), QMessageBox::Ok);
	}
}

void AcademicClassStudentTabWidget::StudentModify()
{
	auto items = academictree->selectedItems();
	if (items.length() != 1) 
	{
		QMessageBox::information(0, tr("student modify"), tr("please select class first"), QMessageBox::Ok);
		return;
	}
	auto item = items[0];
	if (item->parent() != nullptr)
	{
		bool success = false;
		for (int i = 0; i < academicclasstable->rowCount(); i++) 
		{
			QString stuid = academicclasstable->item(i, 0)->text();
			QString stuname = academicclasstable->item(i, 1)->text();
			auto itemsex = (QComboBox*)academicclasstable->cellWidget(i, 2);
			int stusex = itemsex->currentIndex();
			auto itemaca = (QComboBox*)academicclasstable->cellWidget(i, 3);
			QString stuaca = itemaca->currentText();
			auto itemcla = (QComboBox*)academicclasstable->cellWidget(i, 4);
			QString stucla = itemcla->currentText();
			Student stu(stuid, stuname, stusex, stuaca, stucla,"");
			success = dataManager->StudentOP(stu, 1);
		}
		if (success) 
		{
			QMessageBox::information(0, tr("student modify"), tr("student modify successfully"), QMessageBox::Ok);
			studentid->setText("");
			studentname->setText("");
			dataManager->updateStudent();
			updateTree();
		}
		else 
		{
			QMessageBox::information(0, tr("student modify"), tr("student modify failed"), QMessageBox::Ok);
		}
	}
	else 
	{
		QMessageBox::information(0, tr("student modify"), tr("please select class first"), QMessageBox::Ok);
	}
}

void AcademicClassStudentTabWidget::StudentDelete()
{
	if (currenttable == 1) 
	{
		auto currentrow = academicclasstable->currentRow();
		if (currentrow >= 0) 
		{
			auto item = academicclasstable->item(currentrow, 0)->text();
			if (dataManager->StudentOP(Student(item, "", 0, "", "", ""), 2))
			{
				QMessageBox::information(0, tr("student delete"), tr("student delete successfully"), QMessageBox::Ok);
				dataManager->updateStudent();
			}
			else 
			{
				QMessageBox::information(0, tr("student delete"), tr("student delete failed"), QMessageBox::Ok);
			}
		}
		else 
		{
			QMessageBox::information(0, tr("student delete"), tr("please select student first"), QMessageBox::Ok);
		}
	}
	else
	{
		QMessageBox::information(0, tr("student delete"), tr("please select student first"), QMessageBox::Ok);
	}
}

void AcademicClassStudentTabWidget::updateTable(QTreeWidgetItem* item, int col)//更新学院详细信息表格 
{
	academicclasstable->clear();
	academicclasstable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	academicclasstable->setRowCount(0);
	QStringList header;
	auto id = item->text(0).split("-")[0];
	if (item->parent() == nullptr) 
	{
		auto aclass = dataManager->GetAClass();
		academicclasstable->setColumnCount(4);
		header << tr("class id") << tr("class name") << tr("class academic") << tr("student number");
		academicclasstable->setHorizontalHeaderLabels(header);
		for (auto it = aclass->begin(); it != aclass->end(); it++) 
		{
			if (it->GetAcademic() == id) 
			{
				auto rowcount = academicclasstable->rowCount();
				academicclasstable->insertRow(rowcount);
				academicclasstable->setItem(rowcount, 0, new QTableWidgetItem(it->GetID()));
				academicclasstable->setItem(rowcount, 1, new QTableWidgetItem(it->GetName()));
				QComboBox* academicbox = new QComboBox;

				for (auto ita = dataManager->GetAcademics()->begin(); ita != dataManager->GetAcademics()->end(); ita++) 
				{
					academicbox->addItem(ita->GetID());
				}
				academicbox->setCurrentText(it->GetAcademic());
				//academicclasstable->setItem(rowcount, 2, new QTableWidgetItem(it->GetAcademic()));
				academicclasstable->setCellWidget(rowcount, 2, academicbox);
				auto studentnumber = 0;
				for (auto iter = dataManager->GetStudent()->begin(); iter != dataManager->GetStudent()->end(); iter++) 
				{
					if (iter->GetAclass() == it->GetID()) 
					{
						studentnumber++;
					}
				}
				academicclasstable->setItem(rowcount, 3, new QTableWidgetItem(QString::number(studentnumber, 10)));
			}
		}
		currenttable = 0;
		for (int i = 0; i < academicclasstable->rowCount(); i++)
		{
			academicclasstable->item(i, 0)->setFlags(academicclasstable->item(i, 0)->flags() & (~Qt::ItemIsEditable));
			academicclasstable->item(i, 3)->setFlags(academicclasstable->item(i, 3)->flags() & (~Qt::ItemIsEditable));
		}
	}
	else
	{
		auto student = dataManager->GetStudent();
		academicclasstable->setColumnCount(11);
		header << tr("student id") << tr("student name") << tr("student sex") << tr("student academic")<<tr("student class")<<tr("student figure")
			<<tr("in times") << tr("le times") << tr("ab times")
			<<tr("to times")<<tr("on rate");
		academicclasstable->setHorizontalHeaderLabels(header);
		for (auto it = student->begin(); it != student->end(); it++) 
		{
			if (it->GetAclass() == id) 
			{
				auto rowcount = academicclasstable->rowCount();
				academicclasstable->insertRow(rowcount);
				academicclasstable->setItem(rowcount, 0, new QTableWidgetItem(it->GetID()));
				academicclasstable->setItem(rowcount, 1, new QTableWidgetItem(it->GetName()));
				QComboBox* stusexbox = new QComboBox;
				stusexbox->addItems(QStringList() << tr("male") << tr("female"));
				stusexbox->setCurrentText((it->GetSex() == 0) ? tr("male") : tr("female"));
				QComboBox* stuacabox = new QComboBox;
				for (auto ita = dataManager->GetAcademics()->begin(); ita != dataManager->GetAcademics()->end(); ita++)
				{
					stuacabox->addItem(ita->GetID());
				}
				stuacabox->setCurrentText(it->GetAcademic());
				QComboBox* stuclabox = new QComboBox;
				for (auto ita = dataManager->GetAClass()->begin(); ita != dataManager->GetAClass()->end(); ita++) 
				{
					stuclabox->addItem(ita->GetID());
				}
				stuclabox->setCurrentText(it->GetAclass());
				academicclasstable->setCellWidget(rowcount, 2, stusexbox);
				academicclasstable->setCellWidget(rowcount, 3, stuacabox);
				academicclasstable->setCellWidget(rowcount, 4, stuclabox);
				academicclasstable->setItem(rowcount, 5, new QTableWidgetItem(it->GetFigure()));
				academicclasstable->setItem(rowcount, 6, new QTableWidgetItem(QString::number(it->GetIntimes())));
				academicclasstable->setItem(rowcount, 7, new QTableWidgetItem(QString::number(it->GetLetimes())));
				academicclasstable->setItem(rowcount, 8, new QTableWidgetItem(QString::number(it->GetAbtimes())));
				academicclasstable->setItem(rowcount, 9, new QTableWidgetItem(QString::number(it->GetTotimes())));
				academicclasstable->setItem(rowcount, 10, new QTableWidgetItem(QString::number(((double)(it->GetIntimes()))/((double)(it->GetTotimes()))*100.0,'f',2)+"%"));
			}
		}
		currenttable = 1;
		for (int i = 0; i < academicclasstable->rowCount(); i++)
		{
			academicclasstable->item(i, 0)->setFlags(academicclasstable->item(i, 0)->flags() & (~Qt::ItemIsEditable));
			academicclasstable->item(i, 5)->setFlags(academicclasstable->item(i, 5)->flags() & (~Qt::ItemIsEditable));
			academicclasstable->item(i, 6)->setFlags(academicclasstable->item(i, 6)->flags() & (~Qt::ItemIsEditable));
			academicclasstable->item(i, 7)->setFlags(academicclasstable->item(i, 7)->flags() & (~Qt::ItemIsEditable));
			academicclasstable->item(i, 8)->setFlags(academicclasstable->item(i, 8)->flags() & (~Qt::ItemIsEditable));
			academicclasstable->item(i, 9)->setFlags(academicclasstable->item(i, 9)->flags() & (~Qt::ItemIsEditable));
			academicclasstable->item(i, 10)->setFlags(academicclasstable->item(i, 10)->flags() & (~Qt::ItemIsEditable));
		}
	}
}

//根据所选择的学院，来加载该学院的所有班级
void AcademicClassStudentTabWidget::StudentAcademicClassChange(int index)

{
	//首先先清楚班级列表
	studentclass->clear();

	//将当前所选择的学院名称以 ‘-’分隔，取出该编号
	auto s = studentacademic->itemText(index).split("-")[0];
	//遍历该编号
	for (auto it = dataManager->GetAClass()->begin(); it != dataManager->GetAClass()->end(); it++) 
	{
		//当找到该编号时
		if (it->GetAcademic() == s) 
		{
			studentclass->addItem(it->GetID() + "-" + it->GetName());
		}
	}
}

void AcademicClassStudentTabWidget::setupUi()
{
	academictree = new QTreeWidget;//树形控件
	academictree->setColumnCount(1);
	academictree->setMaximumWidth(200);
	academictree->setHeaderLabel(tr("academic class information"));//标题
	updateTree();

	academicclasstable = new QTableWidget;


	//学院管理
	QGroupBox* academicgroup = new QGroupBox(tr("academic administrate"));
	QVBoxLayout* academiclayout = new QVBoxLayout;
	QHBoxLayout* academicinfo = new QHBoxLayout;
	QLabel* labelacademicid = new QLabel(tr("academic id"));
	QLabel* labelacademicname = new QLabel(tr("academic name"));

	academicid = new QLineEdit;//学院编号
	academicid->setMaxLength(4);

	academicname = new QLineEdit;//学院名称
	academicinfo->addWidget(labelacademicid);
	academicinfo->addWidget(academicid);
	academicinfo->addStretch(1);
	academicinfo->addWidget(labelacademicname);
	academicinfo->addWidget(academicname);
	academicinfo->addStretch(3);
	QHBoxLayout* academicop = new QHBoxLayout;
	academicadd = new QPushButton(tr("academic add"));//添加按钮
	academicadd->setMinimumWidth(100);
	/*academicmodify = new QPushButton(tr("academic modify"));
	academicmodify->setMinimumWidth(100);*/
	academicdelete = new QPushButton(tr("academic delete"));//学院删除
	academicdelete->setMinimumWidth(100);

	//布局
	academicop->addStretch(2);
	academicop->addWidget(academicadd);
	//academicop->addWidget(academicmodify);
	academicop->addWidget(academicdelete);
	academiclayout->addLayout(academicinfo);
	academiclayout->addLayout(academicop);
	academicgroup->setLayout(academiclayout);
	

	//班级管理
	QGroupBox* classgroup = new QGroupBox(tr("class administrate"));
	QVBoxLayout* classlayout = new QVBoxLayout;
	QHBoxLayout* classinfo = new QHBoxLayout;
	QLabel* labelclassid = new QLabel(tr("class id"));
	QLabel* labelclassacademic = new QLabel(tr("class academic"));
	QLabel* labelclassname = new QLabel(tr("class name"));
	classid = new QLineEdit;
	classid->setMaxLength(4);
	classname = new QLineEdit;
	classacademic = new QComboBox;
	
	classinfo->addWidget(labelclassid);
	classinfo->addWidget(classid);
	classinfo->addStretch(1);
	classinfo->addWidget(labelclassname);
	classinfo->addWidget(classname);
	classinfo->addStretch(1);
	classinfo->addWidget(labelclassacademic);
	classinfo->addWidget(classacademic);
	classlayout->addLayout(classinfo);

	classadd = new QPushButton(tr("class add"));
	classadd->setMinimumWidth(100);
	classmodify = new QPushButton(tr("class modify"));//修改
	classmodify->setMinimumWidth(100);
	classdelete = new QPushButton(tr("class delete"));
	classdelete->setMinimumWidth(100);

	//布局
	QHBoxLayout* classop = new QHBoxLayout;
	classop->addStretch(2);
	classop->addWidget(classadd);
	classop->addWidget(classmodify);
	classop->addWidget(classdelete);
	classlayout->addLayout(classop);
	classgroup->setLayout(classlayout);


	//学生信息管理
	QGroupBox* studentgroup = new QGroupBox(tr("student administrate"));
	QVBoxLayout* studentlayout = new QVBoxLayout;
	QHBoxLayout* studentinfo = new QHBoxLayout;
	QHBoxLayout* studentinfo2 = new QHBoxLayout;
	QLabel* labelstudentid = new QLabel(tr("student id"));
	QLabel* labelstudentname = new QLabel(tr("student name"));
	QLabel* labelstudentsex = new QLabel(tr("student sex"));
	QLabel* labelstudentacademic = new QLabel(tr("student academic"));
	QLabel* labelstudentclass = new QLabel(tr("student class"));
	
	studentid = new QLineEdit;
	studentid->setMaxLength(8);
	studentname = new QLineEdit;
	studentsex = new QComboBox;
	studentsex->addItems(QStringList() << tr("male") << tr("female"));
	studentclass = new QComboBox;
	studentacademic = new QComboBox;


	//布局
	studentinfo->addWidget(labelstudentid);
	studentinfo->addWidget(studentid);
	studentinfo->addStretch(1);
	studentinfo->addWidget(labelstudentname);
	studentinfo->addWidget(studentname);
	studentinfo->addStretch(1);
	studentinfo->addWidget(labelstudentsex);
	studentinfo->addWidget(studentsex);
	studentlayout->addLayout(studentinfo);
	studentinfo2->addWidget(labelstudentacademic);
	studentinfo2->addWidget(studentacademic);
	studentinfo2->addStretch(1);
	studentinfo2->addWidget(labelstudentclass);
	studentinfo2->addWidget(studentclass);
	studentinfo2->addStretch(3);
	studentlayout->addLayout(studentinfo2);
	//按钮
	studentadd = new QPushButton(tr("student add"));
	studentadd->setMinimumWidth(100);
	studentmodify = new QPushButton(tr("student modify"));
	studentmodify->setMinimumWidth(100);
	studentdelete = new QPushButton(tr("student delete"));
	studentdelete->setMinimumWidth(100);
	QHBoxLayout* studentop = new QHBoxLayout;
	studentop->addStretch(2);
	studentop->addWidget(studentadd);
	studentop->addWidget(studentmodify);
	studentop->addWidget(studentdelete);
	studentlayout->addLayout(studentop);
	studentgroup->setLayout(studentlayout);

	QVBoxLayout* rightlayout = new QVBoxLayout;
	rightlayout->addWidget(academicclasstable);
	rightlayout->addWidget(academicgroup);
	rightlayout->addWidget(classgroup);
	rightlayout->addWidget(studentgroup);

	QHBoxLayout* main_layout = new QHBoxLayout;
	main_layout->addWidget(academictree);
	main_layout->addLayout(rightlayout);
	this->setLayout(main_layout);

	updateComboBox();//更新学院和班级列表

	auto s = studentacademic->currentText().split("-")[0];//获取当前选择的班级编号
	//在班级列表中找到该班级，并加载该学生班级编号和名称
	for (auto it = dataManager->GetAClass()->begin(); it != dataManager->GetAClass()->end(); it++) 
	{
		if (it->GetAcademic() == s) {
			studentclass->addItem(it->GetID() + "-" + it->GetName());
		}
	}
}

void AcademicClassStudentTabWidget::updateTree()//更新信息管理的树形控件
{
	academictree->clear();
	for (auto iter = dataManager->GetAcademics()->begin(); iter != dataManager->GetAcademics()->end(); iter++)
	{
		auto parent = new QTreeWidgetItem(QStringList() << iter->GetID() + "-" + iter->GetName());//加载学院
		for (auto it = dataManager->GetAClass()->begin(); it != dataManager->GetAClass()->end(); it++) 
		{
			if (it->GetAcademic() == iter->GetID())//加载该学院下辖的所有班级
			{
				parent->addChild(new QTreeWidgetItem(QStringList() << it->GetID() + "-" + it->GetName()));
			}
		}
		academictree->addTopLevelItem(parent);
	}
	academictree->expandAll();
}

void AcademicClassStudentTabWidget::updateComboBox()//更新学院信息列表框
{
	classacademic->clear();
	studentacademic->clear();
	//studentclass->clear();
	for (QVector<Academic>::iterator iter = dataManager->GetAcademics()->begin(); iter != dataManager->GetAcademics()->end(); iter++)//将学院信息添加到班级管理、学生管理中的学院列表框列表框中
	{
		classacademic->addItem(iter->GetID()+"-"+iter->GetName());
		studentacademic->addItem(iter->GetID() + "-" + iter->GetName());
	}
	/*auto s = studentacademic->currentText().split("-")[0];
	for (auto it = dataManager->GetAClass()->begin(); it != dataManager->GetAClass()->end(); it++) {
		if (it->GetAcademic() == s) {
			studentclass->addItem(it->GetID() + "-" + it->GetName());
		}
	}*/
}

QString AcademicClassStudentTabWidget::GeneFigureInfo(QString id, QString name)
{
	QByteArray byte_array;
	byte_array.append(id);
	byte_array.append(name);
	QByteArray hash_byte_array = QCryptographicHash::hash(byte_array, QCryptographicHash::Md5);
	return hash_byte_array.toHex();
}
