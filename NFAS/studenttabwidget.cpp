#include "studenttabwidget.hpp"
#include <QHBoxLayout>
#include <QVBoxLayout>

StudentTabWidget::StudentTabWidget(DataManager *dm, QWidget * parent) : QWidget(parent) {
	dataManager = dm;
	setupUi();
}

StudentTabWidget::~StudentTabWidget() {
	
}

void StudentTabWidget::setupUi()
{
	QVBoxLayout* main_layout = new QVBoxLayout;
	this->setLayout(main_layout);
}
