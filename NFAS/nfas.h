#ifndef NFAS_H
#define NFAS_H

#include <QDialog>
#include "DataManager.h"
#include "deal.hpp"
#include <QCloseEvent>

class QTabWidget;


//主窗体
class NFAS : public QDialog
{
	Q_OBJECT

public:
	NFAS(DataManager *dm, QWidget *parent = 0);
	~NFAS();


	static void ExeMingLingProc(LPVOID another);//处理考勤设备的命令线程 
	void closeEvent(QCloseEvent *event);    // 重写closeEvent的申明

public slots:
	void UpdateTab(int index);


private:
	void setupUi();
	QTabWidget* main_tab;//选项卡控件
	DataManager* dataManager;

	bool IsNetWork;//判断本客户端是否有网络通讯功能

	
};

#endif // NFAS_H
