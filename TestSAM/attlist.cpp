#include "attlist.h"
#include <QHeaderView>
#include <QVBoxLayout>

attlist::attlist(QVector<STUDENT> * attlist, QWidget *parent)
	:attendlist(attlist), QDialog(parent)
{
	SetupUi();
}

attlist::~attlist()
{

}

void attlist::SetupUi()
{

	this->setWindowTitle(tr("Attendance List"));

	attendtable = new QTableWidget;

	attendtable->clear();//
	attendtable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	attendtable->setRowCount(0);
	attendtable->setColumnCount(3);

	QStringList header;
	//QStringLiteral
	header << tr("Student Id") << tr("Student Finger") << tr("Sign In");
	attendtable->setHorizontalHeaderLabels(header);

	int i = 0;
	for (auto it = attendlist->begin(); it != attendlist->end(); it++, i++)
	{
		int rowcount = attendtable->rowCount();
		attendtable->insertRow(rowcount);
		attendtable->setItem(rowcount, 0, new QTableWidgetItem(it->StuId));
		attendtable->setItem(rowcount, 1, new QTableWidgetItem(it->StuFinger));

		if (it->StuSign)
			attendtable->setItem(rowcount, 2, new QTableWidgetItem("Yes"));
		else
			attendtable->setItem(rowcount, 2, new QTableWidgetItem("No"));

		//ÉèÖÃ¸ñÊ½
		//attendtable->item(i, 0)->setFlags(attendtable->item(i, 0)->flags() & (~Qt::ItemIsEditable));
		//attendtable->item(i, 1)->setFlags(attendtable->item(i, 1)->flags() & (~Qt::ItemIsEditable));
		attendtable->item(i, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		attendtable->item(i, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		attendtable->item(i, 2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);


		attendtable->item(i, 0)->setFlags(attendtable->item(i, 0)->flags() & (~Qt::ItemIsEditable));
		attendtable->item(i, 1)->setFlags(attendtable->item(i, 1)->flags() & (~Qt::ItemIsEditable));
		attendtable->item(i, 2)->setFlags(attendtable->item(i, 1)->flags() & (~Qt::ItemIsEditable));
	}


	QVBoxLayout * main_layout = new QVBoxLayout;

	main_layout->addWidget(attendtable);
	this->setLayout(main_layout);
}
