#include "nfas.h"
#include <QtWidgets/QApplication>
#include <QTranslator>
#include "DataManager.h"
#include "logindialog.hpp"
#include <TcpServer.h>
#include <QDir>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTranslator qtTranslator;
	qtTranslator.load(":/NFAS/nfas_zh.qm");//加载语言翻译文件
	a.installTranslator(&qtTranslator);

	QString config = QCoreApplication::applicationDirPath() + "/config.ini";
	//调试

	//QApplication::desktop()->availableGeometry();
	//-------------------


	//创建本程序所用到的所有数据管理类
	DataManager dataManager(config);

	LoginDialog logindialog(&dataManager);

	logindialog.setModal(true);//设置模态类型

	if (logindialog.exec() != QDialog::Accepted)
	{
		return -1;
	}
	NFAS w(&dataManager);//加载主界面
	w.show();
	return a.exec();
}
