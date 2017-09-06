#ifndef NFAS_H
#define NFAS_H

#include <QDialog>
#include "DataManager.h"

class QTabWidget;


//主窗体
class NFAS : public QDialog
{
	Q_OBJECT

public:
	NFAS(DataManager *dm, QWidget *parent = 0);
	~NFAS();

public slots:
	void UpdateTab(int index);

private:
	void setupUi();
	QTabWidget* main_tab;//选项卡控件
	DataManager* dataManager;
};

#endif // NFAS_H
