#pragma once
#include <QWidget>
#include "ui_devicewidget.h"
#include "DataManager.h"
class devicewidget : public QWidget 
{
	Q_OBJECT

public:
	devicewidget(DataManager* dm , QWidget * parent = 0);
	~devicewidget();
	void updateTable();
	void UpdateTab();

private:

	
	
	DataManager* dataManager;

	Ui::Form ui;
};
