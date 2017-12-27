#ifndef USERTABWIDGET_H
#define USERTABWIDGET_H
#include <QWidget>
#include "DataManager.h"
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QComboBox>

class UserTabWidget : public QWidget 
{
	Q_OBJECT

public:
	UserTabWidget(DataManager *dm, QWidget * parent = 0);
	~UserTabWidget();

	void UpdateTab();

public slots:
//增删改
	void UserAdd();
	void UserModify();
	void UserDelete();

private:
	void updateTable();
	void setupUi();
	DataManager* dataManager;
	QTableWidget* usertable;
	QLineEdit* userid;
	QLineEdit* userpsd;
	QLineEdit* userpsdagain;
	QComboBox* useridentify;
	QPushButton* useradd;
	QPushButton* usermodify;
	QPushButton* userdelete;

};
#endif // USERTABWIDGET_H