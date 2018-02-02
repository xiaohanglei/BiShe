#include "attendance.h"
#include <QPushButton>
AttendanceM	::AttendanceM(TcpClient * tcpc, int method, QWidget *parent)
	:tcpclient(tcpc), m_method(method),QWidget(parent)
{
	Stuends = new QVector<STUDENT>;
	isLeisure = true;//初始空闲状态
	iscomplete = false;//
	//ui.setupUi(this);
	SetupUi();

	connect(buttok, SIGNAL(clicked()), this, SLOT(slotSign()));
	QObject::connect(tcpclient->GetSock(), &QTcpSocket::readyRead, this, &AttendanceM::RecvHuiZhiPro);
}

AttendanceM::~AttendanceM()
{

}
void AttendanceM::SendOrderPro(PVOID another)
{
	while (1)
	{
		Sleep(60000 * 5);//分钟
		if (isLeisure)//是否空闲
		{
			//发送请求待考勤名单命令

			//等待回执
			int waittime = 0;
			while (!isrecvdata)
			{
				Sleep(1000 * 5);
				waittime++;

				if (waittime == 5 * 12 * 2)//如果超过2分钟则放弃等待
					break;
				
			}
			if (isrecvdata)//收到回执
			{
				isLeisure = false;//将状态设置非空闲

				//准备开始考勤
			}
			
		}
		else//如果不是空闲状态，则判断是否考勤完毕
		{
			if (iscomplete)
			{
				//推送考勤结果命令

				//等待回执

			}
			continue;
		}
	}
}
void AttendanceM::RecvHuiZhiPro()
{
	//
	QByteArray recvbyte;

	recvbyte = tcpclient->GetSock()->readAll();


	int recvlen = recvbyte.size();
	UCHAR * netbao = 0;
	netbao = (UCHAR *)recvbyte.data();

	if (netbao[0] == 0xff)
	{
		QMessageBox::information(0, tr("Success"), tr("Recv Data"), QMessageBox::Ok);
	}

}
void AttendanceM::slotSign()
{
	QString input = editinput->text();

	for (auto it = Stuends->begin(); it != Stuends->end(); it++)
	{
		if (m_method == 0)//学号签到
		{
			if (input == it->StuId)
			{
				it->StuSign = true;
			}
		}
		else//指纹签到
		{
			if (input == it->StuFinger)
			{
				it->StuSign = true;
			}

		}
	}

}


void AttendanceM::SetupUi()
{
	this->setWindowTitle(tr("Attendance Machine Simulator"));
	this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint/* | Qt::WindowMaximizeButtonHint*//*| Qt::Widget | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint*/);//关闭和最小化按钮
																																																					  //this->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);

	this->setFixedSize(QSize(400, 200));



	editinput = new QLineEdit;
	QLabel * label1;
	if (m_method == 0)//学号签到
	{
		label1 = new QLabel(tr("StuId"));
	}
	else
	{
		label1 = new QLabel(tr("Finger"));
	}
	buttok = new QPushButton(tr("submit"));
	QHBoxLayout * layout_border = new QHBoxLayout;

	layout_border->addStretch(1);
	layout_border->addWidget(label1);
	layout_border->addWidget(editinput);
	layout_border->addWidget(buttok);
	layout_border->addStretch(1);

	QVBoxLayout * layout_main = new QVBoxLayout;
	layout_main->addStretch(1);
	layout_main->addLayout(layout_border);
	layout_main->addStretch(1);


	this->setLayout(layout_main);



}
