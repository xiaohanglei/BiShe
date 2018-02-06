#include "attendance.h"
#include <QPushButton>
#include <windows.h>
#include <process.h>
AttendanceM	::AttendanceM(QString clroom, TcpClient * tcpc, int method, QWidget *parent)
	: classroom(clroom),tcpclient(tcpc), m_method(method),QWidget(parent)
{
	attendance = new ATTEND;
	already_attend = new QVector<ATTEND>;
	isLeisure = true;//初始空闲状态
	iscomplete = false;//
	isrecvdata = false;
	//ui.setupUi(this);
	SetupUi();

	connect(buttok, SIGNAL(clicked()), this, SLOT(attend()));
	QObject::connect(tcpclient->GetSock(), &QTcpSocket::readyRead, this, &AttendanceM::RecvHuiZhiPro);
	QObject::connect(&deal, &CDeal::RecvAttenance, this, &AttendanceM::slotSign);

	//SendReQuest(this);
	_beginthread(SendOrderPro, 0, this);
	
}

AttendanceM::~AttendanceM()
{

}
void AttendanceM::SendOrderPro(PVOID another)
{
	AttendanceM * attend = (AttendanceM *)another;

	while (1)
	{
		//Sleep(60000 * 5);//3分钟
		Sleep(10000);//10s
		if (attend->isLeisure)//是否空闲
		{
			//发送请求待考勤名单命令
			SendReQuest(attend);

			//等待回执
			int waittime = 0;
			while (!attend->isrecvdata)
			{
				Sleep(1000 * 5);
				waittime++;

				if (waittime == 12 * 2)//如果超过2分钟则放弃等待
					break;
				
			}
			if (attend->isrecvdata)//收到回执
			{
				attend->isrecvdata = false;

				//当考勤名单中的学生数量不等于0   并且  该考勤项目没有考勤过，才能考勤(考勤过滤)
				if (attend->attendance->Stuends.size() != 0 && !attend->AttendFiltration())
				{
					attend->isLeisure = false;//将状态设置非空闲
					
					//准备开始考勤()
					AttendPro(attend);
				}
				
			}
			
		}
		else//如果不是空闲状态，则判断是否考勤完毕
		{
			if (attend->iscomplete)
			{
				//推送考勤结果命令

				SendResult(attend);

				//清空当前考勤项目内容
				attend->attendance->signcount = 0;
				attend->attendance->Stuends.clear();//清空考勤名单，只保留考勤编号和考勤时段

				attend->already_attend->append(*attend->attendance);//将只有考勤编号和考勤时段的考勤项目存入已考勤列表，以便过滤相同考勤项目


				attend->attendance->attendanceid = "0";//然后清空剩下的数据信息
				attend->attendance->starttime = 0;
				attend->attendance->endtiem = 0;
				
				

				attend->isLeisure = true;//将状态设置空闲

				attend->iscomplete = false;


			}
			continue;
		}
	}
}

void AttendanceM::SendReQuest(AttendanceM * another)
{
	UCHAR * ptrSendData = new UCHAR[MAX_BUFF];
	int sendlen = 0;
	ptrSendData[4] = 0x8f;

	char mdzm[7] = "Server";
	char *sozm = nullptr;
	QByteArray tempQB = another->classroom.toLatin1();
	sozm = tempQB.data();

	memcpy(&ptrSendData[5], mdzm, 6);
	memcpy(&ptrSendData[11], sozm, 6);
	ptrSendData[17] = 0x01;
	ptrSendData[18] = 0x11;

	sendlen += 14;

	ptrSendData[sendlen + 5] = 0xff;
	sendlen += 1;
	memcpy(&ptrSendData[0], &sendlen, 4);

	another->tcpclient->GetSock()->write(((char *)ptrSendData),sendlen + 5);
	another->tcpclient->GetSock()->waitForBytesWritten();

	delete[]ptrSendData;

}

void AttendanceM::SendResult(AttendanceM * another)
{
	UCHAR * ptrSendData = new UCHAR[MAX_BUFF];
	int sendlen = 0;
	ptrSendData[4] = 0x8f;

	char mdzm[7] = "Server";
	char *sozm = nullptr;
	QByteArray tempQB = another->classroom.toLatin1();
	sozm = tempQB.data();

	memcpy(&ptrSendData[5], mdzm, 6);
	memcpy(&ptrSendData[11], sozm, 6);
	ptrSendData[17] = 0x01;
	ptrSendData[18] = 0x12;

	sendlen += 14;

	//考勤项目编号
	QByteArray tempQBattid = another->attendance->attendanceid.toLatin1();
	char * attid = tempQBattid.data();

	memcpy(&ptrSendData[19], attid, 8);

	//记录数
	int count = another->attendance->Stuends.size();
	memcpy(&ptrSendData[27], &count, 2);

	sendlen += 10;

	QByteArray tempQBSid;
	char * tempid = nullptr;
	for (int i = 0; i < count; i++)
	{
		tempQBSid = another->attendance->Stuends.at(i).StuId.toLatin1();
		tempid = tempQBSid.data();

		memcpy(&ptrSendData[29 + i * 9], tempid, 8);

		if (another->attendance->Stuends.at(i).StuSign)
			ptrSendData[37 + i * 9] = 0x01;
		else
			ptrSendData[37 + i * 9] = 0x00;
	}
	sendlen += count * 9;

	ptrSendData[sendlen + 5] = 0xff;
	sendlen += 1;
	memcpy(&ptrSendData[0], &sendlen, 4);

	another->tcpclient->GetSock()->write(((char *)ptrSendData), sendlen + 5);
	another->tcpclient->GetSock()->waitForBytesWritten();

	delete[]ptrSendData;

}


void AttendanceM::RecvHuiZhiPro()
{
	//
	QByteArray recvbyte;

	recvbyte = tcpclient->GetSock()->readAll();

	int recvlen = recvbyte.size();
	UCHAR * netbao = 0;
	netbao = (UCHAR *)recvbyte.data();

	
	deal.FenLiZhen(netbao, recvlen, attendance);//调用处理模块将接受到的数据分帧 


}
void AttendanceM::attend()
{
#if 1
	QString input = editinput->text();

	for (auto it = attendance->Stuends.begin(); it != attendance->Stuends.end(); it++)
	{
		if (m_method == 0)//学号签到
		{
			if (input == it->StuId)
			{
				it->StuSign = true;
				attendance->signcount++;//签到一个人，记录一下
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
#endif
}
void AttendanceM::slotSign()
{
	//
	isrecvdata = true;//收到数据


}

void AttendanceM::AttendPro(AttendanceM * another)
{
	//

	int currtime = QDateTime::currentDateTime().toTime_t();//当前时间的时间戳

	//
	Sleep(another->attendance->starttime - currtime);//延时等待到开始时间



	while (1)
	{
		Sleep(5000);
		currtime = QDateTime::currentDateTime().toTime_t();//当前时间的时间戳
		if ( currtime > another->attendance->endtiem || another->attendance->signcount == another->attendance->Stuends.size())//当考勤时间结束或者，已经全部签到完毕以后
		{
			another->iscomplete = true;//完成考勤
			break;
		}
	}	
}

bool AttendanceM::AttendFiltration()
{
	for (auto it = already_attend->begin(); it != already_attend->end(); it++)
	{
		if (it->attendanceid == attendance->attendanceid)
		{
			if (it->starttime == attendance->starttime)
			{
				if (it->endtiem == attendance->endtiem)
				{
					return true;
				}
			}
		}
	}
	
	return false;
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
