#include "testsam.h"
#include <QValidator>
#include "attendance.h"
#include <QtWidgets/QApplication>
TestSAM::TestSAM(QWidget *parent)
	: QWidget(parent)
{
	tcpclient = new TcpClient;
	SetupUi();
	connect(submit, SIGNAL(clicked()), this, SLOT(slotSubmit()));
	//connect(buttTestSendMsg, SIGNAL(clicked()), this, SLOT(slotTestSend()));
	//QObject::connect(tcpclient->GetSock(), &QTcpSocket::readyRead, this, &TestSAM::RecvMsg);
	//QObject::connect(tcpclient->GetSock(), &QTcpSocket::disconnected, this, &TestSAM::slotTestSend);
}

TestSAM::~TestSAM()
{

}

void TestSAM::slotSubmit()
{
	
	int method = 0;
	if (!(radioStuId->isChecked() || radioTable->isChecked()))
	{
		QMessageBox::information(0, tr("Error"), tr("Please Select Attendance Method"), QMessageBox::Ok);
		return;
	}
	if (radioTable->isChecked())
		method = 1;

	QStringList bip = editServerIp->text().split(".");
	if (bip.size()!= 4)
	{
		QMessageBox::information(0, tr("Error"), tr("Ip Address Error"), QMessageBox::Ok);
		return;
	}
	for (int i = 0; i < bip.size(); i++)
	{
		if (bip.at(i).toInt() > 255)
		{
			QMessageBox::information(0, tr("Error"), tr("Ip Address Error"), QMessageBox::Ok);
			return;
		}
	}

	if (editServerPort->text().toInt() < 1024)//在界面中以及对端口范围进行限制
	{
		QMessageBox::information(0, tr("Error"), tr("Port Address Error"), QMessageBox::Ok);
		return;
	}

	if (editAttendClassRoom->text().length() != 6)
	{
		QMessageBox::information(0, tr("Error"), tr("Class Room Error"), QMessageBox::Ok);
		return;
	}
	
	tcpclient->SetIp(editServerIp->text());
	tcpclient->SetPort(editServerPort->text().toInt());

	if (!tcpclient->ConnectServer())
	{
		QMessageBox::information(0, tr("Error"), tr("Server Connection failed"), QMessageBox::Ok);
		return;
	}
	//QMessageBox::information(0, tr("Error"), tr("Server Connection successful"), QMessageBox::Ok);

	submit->setEnabled(false);
	AttendanceM *attendm = new AttendanceM(editAttendClassRoom->text(),tcpclient,method);
	this->hide();
	attendm->show();
		
}
void TestSAM::SendXinTiao()
{
	
}

void TestSAM::slotTestSend()
{
	QMessageBox::StandardButton button;
	button = QMessageBox::information(this, tr("Exit"), QString(tr("Server Disconnect,Reconnectint")), QMessageBox::Ok );

	int waitcount = 0;

	while (!tcpclient->ConnectServer())
	{
		Sleep(1000 * 10);
		waitcount++;

		if (waitcount > 30)//5分钟都重连不上，则直接退出程序
		{
			button = QMessageBox::information(this, tr("Exit"), QString(tr("Reconnectint Failed")), QMessageBox::Ok);
			QApplication::exit();
		}

	}


	/*
	while (button == QMessageBox::Yes)
	{
		if (tcpclient->ConnectServer())
		{
			break;
		}
		else
		{
			button = QMessageBox::question(this, tr("Exit"), QString(tr("Reconnect Failed,Is Continue Reconnect?")), QMessageBox::Yes | QMessageBox::No);
		}
	}

	if (button == QMessageBox::No)
	{
		QApplication::exit();
	}
	*/
}
void TestSAM::RecvMsg()
{
	QByteArray recvbyte;

	recvbyte = tcpclient->GetSock()->readAll();

	QString showmsg = recvbyte;

	if (showmsg[1] == 0x65)
		QMessageBox::information(0, tr("Error"), showmsg, QMessageBox::Ok);

	//showmsg.at(2).
		
}

void TestSAM::SetupUi()
{
	this->setWindowTitle(tr("Attendance Machine Simulator"));
	this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint/* | Qt::WindowMaximizeButtonHint*//*| Qt::Widget | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint*/);//关闭和最小化按钮
																																																				  //this->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);

																																											this->setFixedSize(QSize(400,200));

	//this->setGeometry(400, 320, 400, 200);
	//this->setFixedSize(QSize(a.width(), a.height()-100));
	//this->showFullScreen();//全屏显示
	//this->showMaximized();	

	main_layout = new QHBoxLayout;

	editServerIp = new QLineEdit;
	editServerIp->setMaxLength(15);
	editServerIp->setText("127.0.0.1");
	

	editServerPort = new QLineEdit;
	editServerPort->setMaxLength(15);
	editServerPort->setValidator(new QIntValidator(1024, 65536, this));
	editServerPort->setMaximumWidth(40);
	editServerPort->setText("9696");

	editAttendClassRoom = new QLineEdit;
	editAttendClassRoom->setText("10-211");
	groupAttend = new QButtonGroup;//(tr("Attendance Method"))

	radioTable = new QRadioButton(tr("Table"));
	radioStuId = new QRadioButton(tr("StuId"));
	

	QLabel * labelIp = new QLabel(tr("Server Ip"));
	QLabel * labelPort = new QLabel(tr("Server Port"));
	QLabel * labelClassRoom = new QLabel(tr("Class Room"));

	groupAttend->addButton(radioStuId);
	groupAttend->addButton(radioTable);


	QHBoxLayout * layout_NetWork = new QHBoxLayout;
	layout_NetWork->addWidget(labelIp);
	layout_NetWork->addWidget(editServerIp);
	layout_NetWork->addStretch(2);
	layout_NetWork->addWidget(labelPort);
	layout_NetWork->addWidget(editServerPort);
	layout_NetWork->addStretch(2);

	QHBoxLayout * layout_RoomMethod = new QHBoxLayout;
	layout_RoomMethod->addWidget(labelClassRoom);
	layout_RoomMethod->addWidget(editAttendClassRoom);

	layout_RoomMethod->addStretch(1);

	QGroupBox * groupmethod = new QGroupBox(tr("Attendance Method"));
	QHBoxLayout * layout_Method = new QHBoxLayout;
	layout_Method->addWidget(radioStuId);
	layout_Method->addWidget(radioTable);

	groupmethod->setLayout(layout_Method);

	layout_RoomMethod->addWidget(groupmethod);

	layout_RoomMethod->addStretch(1);

	QVBoxLayout * layout_right = new QVBoxLayout;

	QLabel * title = new QLabel(tr("Environment Setting "));
	QHBoxLayout * layout_title = new QHBoxLayout;

	submit = new QPushButton(tr("Submit"));
	QHBoxLayout * layout_submit = new QHBoxLayout;

	buttTestSendMsg = new QPushButton(tr("Send"));
	buttTestSendMsg->setVisible(false);

	layout_submit->addStretch(1);
	layout_submit->addWidget(submit);
	layout_submit->addStretch(1);
	layout_submit->addWidget(buttTestSendMsg);
	layout_submit->addStretch(1);

	layout_title->addStretch(1);
	layout_title->addWidget(title);
	layout_title->addStretch(1);

	layout_right->addStretch(1);
	layout_right->addLayout(layout_title);
	layout_right->addStretch(1);
	layout_right->addLayout(layout_NetWork);
	layout_right->addStretch(1);
	layout_right->addLayout(layout_RoomMethod);
	layout_right->addStretch(2);
	layout_right->addLayout(layout_submit);
	layout_right->addStretch(3);


	

	main_layout->addLayout(layout_right);
	//main_layout->addStretch(1);
	this->setLayout(main_layout);


}
