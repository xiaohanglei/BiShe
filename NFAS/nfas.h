#ifndef NFAS_H
#define NFAS_H

#include <QDialog>
#include "DataManager.h"
#include "deal.hpp"

class QTabWidget;


//主窗体
class NFAS : public QDialog
{
	Q_OBJECT

public:
	NFAS(DataManager *dm, QWidget *parent = 0);
	~NFAS();


	static void ExeMingLingProc(LPVOID another);//处理考勤设备的命令线程 

public slots:
	void UpdateTab(int index);

private:
	void setupUi();
	QTabWidget* main_tab;//选项卡控件
	DataManager* dataManager;
};

#endif // NFAS_H
