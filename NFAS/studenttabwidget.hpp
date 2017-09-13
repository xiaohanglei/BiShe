#ifndef STUDENTTABWIDGET_H
#define STUDENTTABWIDGET_H
#include <QWidget>
#include "DataManager.h"

class StudentTabWidget : public QWidget {
	Q_OBJECT

public:
	StudentTabWidget(DataManager *dm, QWidget * parent = 0);
	~StudentTabWidget();

private:
	void setupUi();
	DataManager* dataManager;
};
#endif // STUDENTTABWIDGET_H