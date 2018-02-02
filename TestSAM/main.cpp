#include "testsam.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	TestSAM w;
	w.show();
	return a.exec();
}
