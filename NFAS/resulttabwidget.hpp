#ifndef RESULTTABWIDGET_H
#define RESULTTABWIDGET_H
#include <QWidget>
#include "DataManager.h"
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QTreeWidget>
//#include <QtXlsx/QtXlsx>
#include <QLabel>
#include "deal.hpp"

#if _MSC_VER > 1000  
#pragma comment(lib,"Qt5Xlsxd.lib")  
#endif  

#if _MSC_VER >= 1600  
#pragma execution_character_set("utf-8")  
#endif  

#ifdef _DEBUG
#include <QComboBox>
#include <QDebug>
#endif


class ResultTabWidget : public QWidget 
{
	Q_OBJECT

public:
	ResultTabWidget(DataManager *dm, QWidget * parent = 0);
	~ResultTabWidget();

	void UpdateTab();

public slots:
	void ResultAdd();
	void ResultAModify();
	void ResultDelete();
	void updateTable(QTreeWidgetItem* item, int col);
	void AttendanceFileGet();
	void CheckChange(QTableWidgetItem *item);//更新表格文件
	void GeneTestData();//生成测试文件

	
private:
	void setupUi();
public:
	void updateTree();
	QLabel* totalnumber;
	QLabel* absencenumber;
	QLabel* leavenumber;
	DataManager* dataManager;
	QLineEdit* attendencefile;
	QPushButton* attendancefileselect;	
	QTreeWidget* resulttree;
	QTableWidget* attendancetable;//考勤結果表 
	QPushButton* resultadd;
	QPushButton* resultmodify;
	QPushButton* resultdelete;

	Result attendanceresult;

	int currenttablefrom = -1;



#ifdef _DEBUG
	QPushButton* debugfile;
	QComboBox* attendance;
#endif
};
#endif // RESULTTABWIDGET_H