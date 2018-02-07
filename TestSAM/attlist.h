#ifndef ATTLIST_H
#define ATTLIST_H

#include <QDialog>
#include <QTableWidget>
#include <deal.h>
//#include "ui_attlist.h"

class attlist : public QDialog
{
	Q_OBJECT

public:
	attlist(QVector<STUDENT> * attlist,QWidget *parent = 0);
	~attlist();

private:

	void SetupUi();

	QTableWidget *attendtable;
	

	QVector<STUDENT> * attendlist;
};

#endif // ATTLIST_H
