#include "testsam.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>  

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
	
	
	QTranslator qtTranslator;
	qtTranslator.load(":/TestSAM/testsam_zh.qm");//º”‘ÿ”Ô—‘∑≠“ÎŒƒº˛
	a.installTranslator(&qtTranslator);


	TestSAM w;
	w.show();
	return a.exec();
}
