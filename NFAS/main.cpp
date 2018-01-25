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
	qtTranslator.load(":/NFAS/nfas_zh.qm");//�������Է����ļ�
	a.installTranslator(&qtTranslator);

	QString config = QCoreApplication::applicationDirPath() + "/config.ini";
	//����

	//QApplication::desktop()->availableGeometry();
	//-------------------


	//�������������õ����������ݹ�����
	DataManager dataManager(config);

	LoginDialog logindialog(&dataManager);

	logindialog.setModal(true);//����ģ̬����

	if (logindialog.exec() != QDialog::Accepted)
	{
		return -1;
	}
	NFAS w(&dataManager);//����������
	w.show();
	return a.exec();
}
