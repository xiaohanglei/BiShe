#include "testsam.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QTranslator qtTranslator;
	qtTranslator.load(":/TestSAM/testsam_zh.qm");//¼ÓÔØÓïÑÔ·­ÒëÎÄ¼ş
	a.installTranslator(&qtTranslator);

	TestSAM w;
	w.show();
	return a.exec();
}
