#ifndef ACADEMICCLASSSTUDENTTABWIDGET_H
#define ACADEMICCLASSSTUDENTTABWIDGET_H
#include <QWidget>
#include "DataManager.h"
#include <QTreeWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>

class AcademicClassStudentTabWidget : public QWidget 
{
	Q_OBJECT

public:
	AcademicClassStudentTabWidget(DataManager *dm, QWidget * parent = 0);
	~AcademicClassStudentTabWidget();

	void UpdateTab();

public slots:
	void AcademicAdd();
	//void AcademicModify();
	void AcademicDelete();
	void ClassAdd();
	void ClassModify();
	void ClassDelete();
	void StudentAdd();
	void StudentModify();
	void StudentDelete();
	void updateTable(QTreeWidgetItem* item, int col);
	void StudentAcademicClassChange(int index);//根据选择的学院来加载该学院下辖的所有班级

private:
	void setupUi();
	void updateTree();
	void updateComboBox();
	
	QString GeneFigureInfo(QString id,QString name);
	//控件
	DataManager* dataManager;
	QTreeWidget* academictree;
	QTableWidget* academicclasstable;

	QLineEdit* academicid;
	QLineEdit* academicname;

	QLineEdit* classid;
	QLineEdit* classname;
	QComboBox* classacademic;

	QLineEdit* studentid;
	QLineEdit* studentname;
	QComboBox* studentsex;
	QComboBox* studentacademic;
	QComboBox* studentclass;

	QPushButton* academicadd;
	//QPushButton* academicmodify;
	QPushButton* academicdelete;
	QPushButton* classadd;
	QPushButton* classmodify;
	QPushButton* classdelete;
	QPushButton* studentadd;
	QPushButton* studentmodify;
	QPushButton* studentdelete;
	int currenttable = -1;
};

#endif // ACADEMICCLASSSTUDENTTABWIDGET_H