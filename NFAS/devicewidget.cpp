#include "devicewidget.hpp"

devicewidget::devicewidget(DataManager* dm, QWidget * parent) : dataManager(dm) , QWidget(parent)
{
	ui.setupUi(this);
	updateTable();

}

devicewidget::~devicewidget() 
{
	
}

void devicewidget::updateTable()
{
	ui.devicetable->clear();//
	ui.devicetable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.devicetable->setRowCount(0);
	ui.devicetable->setColumnCount(2);

	QStringList header;
	//QStringLiteral
	header << tr("Class room") << tr("Device IP");
	ui.devicetable->setHorizontalHeaderLabels(header);

	int i = 0;
	for (QVector<Device>::iterator it = dataManager->GetDevice()->begin(); it != dataManager->GetDevice()->end(); it++,i++)
	{
		int rowcount = ui.devicetable->rowCount();
		ui.devicetable->insertRow(rowcount);
		ui.devicetable->setItem(rowcount, 0, new QTableWidgetItem(it->GetName()));
		ui.devicetable->setItem(rowcount, 1, new QTableWidgetItem(it->GetIp()));

		//设置格式
		//ui.devicetable->item(i, 0)->setFlags(ui.devicetable->item(i, 0)->flags() & (~Qt::ItemIsEditable));
		//ui.devicetable->item(i, 1)->setFlags(ui.devicetable->item(i, 1)->flags() & (~Qt::ItemIsEditable));
		ui.devicetable->item(i, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		ui.devicetable->item(i, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);


		ui.devicetable->item(i, 0)->setFlags(ui.devicetable->item(i, 0)->flags() & (~Qt::ItemIsEditable));
		ui.devicetable->item(i, 1)->setFlags(ui.devicetable->item(i, 1)->flags() & (~Qt::ItemIsEditable));
	}

}
