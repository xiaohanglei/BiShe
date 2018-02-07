#include "usertabwidget.hpp"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QMessageBox>

UserTabWidget::UserTabWidget(DataManager *dm, QWidget * parent) : QWidget(parent)
{
	dataManager = dm;
	setupUi();

	connect(useradd, SIGNAL(clicked()), this, SLOT(UserAdd()));
	connect(usermodify, SIGNAL(clicked()), this, SLOT(UserModify()));
	connect(userdelete, SIGNAL(clicked()), this, SLOT(UserDelete()));
}

UserTabWidget::~UserTabWidget()
{

}

void UserTabWidget::UpdateTab()
{
}

void UserTabWidget::UserAdd()
{
	QString uid = userid->text().trimmed();
	QString psd = userpsd->text();
	QString psda = userpsdagain->text();
	int identify = useridentify->currentIndex();
	if (uid.length() <= 0 || uid.length() > 8)
	{
		QMessageBox::information(0, tr("user add"), tr("length of user id must be 1 - 8"), QMessageBox::Ok);
		return;
	}
	if (psd.length() == 0) 
	{
		QMessageBox::information(0, tr("user add"), tr("length of user password is required"), QMessageBox::Ok);
		return;
	}
	if (psd != psda)
	{
		QMessageBox::information(0, tr("user add"), tr("password and passwordagain donot match"), QMessageBox::Ok);
		return;
	}
	if (dataManager->UserOP(User(uid, psd, identify), 0)) 
	{
		QMessageBox::information(0, tr("user add"), tr("user add successfully"), QMessageBox::Ok);
		userid->setText("");
		userpsd->setText("");
		userpsdagain->setText("");
		dataManager->updateUser();
		updateTable();
	}
	else 
	{
		QMessageBox::information(0, tr("user add"), tr("user add failed"), QMessageBox::Ok);
	}
}

void UserTabWidget::UserModify()
{
	bool success = true;
	for (int i = 0; i < usertable->rowCount(); i++) 
	{
		auto uid = usertable->item(i, 0)->text();
		auto identify = ((QComboBox*)(usertable->cellWidget(i, 1)))->currentIndex();
		success = dataManager->UserOP(User(uid, "", identify), 1);
	}
	if (success) 
	{
		QMessageBox::information(0, tr("user modify"), tr("user modify successfully"), QMessageBox::Ok);
		dataManager->updateUser();
	}
	else 
	{
		QMessageBox::information(0, tr("user modify"), tr("user modify failed"), QMessageBox::Ok);
	}
}

void UserTabWidget::UserDelete()
{
	auto row = usertable->currentRow();
	if (row > 0)
	{
		auto uid = usertable->item(row, 0)->text();
		if (dataManager->UserOP(User(uid, "", 0), 2))
		{
			QMessageBox::information(0, tr("user delete"), tr("user delete successfully"), QMessageBox::Ok);
			dataManager->updateUser();
			updateTable();
		}
		else
		{
			QMessageBox::information(0, tr("user delete"), tr("user delete failed"), QMessageBox::Ok);
		}
	}
	else 
	{
		QMessageBox::information(0, tr("user delete"), tr("please select user first"), QMessageBox::Ok);
	}
}

void UserTabWidget::updateTable()
{
	usertable->clear();
	usertable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	usertable->setRowCount(0);
	usertable->setColumnCount(2);
	QStringList header;
	header << tr("user id") << tr("user identify");
	usertable->setHorizontalHeaderLabels(header);
	for (auto it = dataManager->GetUser()->begin(); it != dataManager->GetUser()->end(); it++)
	{
		auto rowcount = usertable->rowCount();
		usertable->insertRow(rowcount);
		usertable->setItem(rowcount, 0, new QTableWidgetItem(it->GetUID()));
		QComboBox* itembox = new QComboBox;
		itembox->addItems(QStringList() << tr("administrator") << tr("teacher"));
		if (it->GetIdentify() == 1) 
		{
			itembox->setCurrentIndex(1);
		}
		usertable->setCellWidget(rowcount, 1, itembox);
		//usertable->setItem(rowcount, 1, new QTableWidgetItem(its->GetName()));
	}
}

void UserTabWidget::setupUi()
{
	usertable = new QTableWidget;
	usertable->setMaximumWidth(1920);


	QLabel* labeluserid = new QLabel(tr("user id"));
	QLabel* labeluserpsd = new QLabel(tr("user password"));
	QLabel* labeluserpsdagain = new QLabel(tr("user password again"));
	QLabel* labeluseridentify = new QLabel(tr("user identify"));
	userid = new QLineEdit;
	userid->setMaxLength(6);
	userpsd = new QLineEdit;
	userpsdagain = new QLineEdit;
	userpsd->setEchoMode(QLineEdit::Password);
	userpsdagain->setEchoMode(QLineEdit::Password);
	useradd = new QPushButton(tr("user add"));
	useradd->setMinimumWidth(100);
	usermodify = new QPushButton(tr("user modify"));
	usermodify->setMinimumWidth(100);
	userdelete = new QPushButton(tr("user delete"));
	userdelete->setMinimumWidth(100);
	useridentify = new QComboBox;
	//权限列表
	useridentify->addItems(QStringList() << tr("administrator") << tr("teacher"));
	QHBoxLayout* idlayout = new QHBoxLayout;
	idlayout->addWidget(labeluserid);
	idlayout->addWidget(userid);
	idlayout->addStretch(1);
	idlayout->addWidget(labeluseridentify);
	idlayout->addWidget(useridentify);
	idlayout->addStretch(3);
	QHBoxLayout* psdlayout = new QHBoxLayout;
	psdlayout->addWidget(labeluserpsd);
	psdlayout->addWidget(userpsd);
	psdlayout->addStretch(2);
	QHBoxLayout* psdagalayout = new QHBoxLayout;
	psdagalayout->addWidget(labeluserpsdagain);
	psdagalayout->addWidget(userpsdagain);
	psdagalayout->addStretch(2);
	QHBoxLayout* oplayout = new QHBoxLayout;
	oplayout->addStretch(3);
	oplayout->addWidget(useradd);
	oplayout->addWidget(usermodify);
	oplayout->addWidget(userdelete);

	QVBoxLayout* main_layout = new QVBoxLayout;
	main_layout->addWidget(usertable);
	main_layout->addLayout(idlayout);
	main_layout->addLayout(psdlayout);
	main_layout->addLayout(psdagalayout);
	main_layout->addLayout(oplayout);

	QHBoxLayout* H_main_layout = new QHBoxLayout;
	H_main_layout->addStretch(1);
	H_main_layout->addLayout(main_layout);
	H_main_layout->addStretch(1);

	this->setLayout(H_main_layout);

	updateTable();
}
