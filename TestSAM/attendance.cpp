#include "attendance.h"
#include <QPushButton>
#include <windows.h>
#include <process.h>
#include <attlist.h>
AttendanceM	::AttendanceM(QString clroom, TcpClient * tcpc, int method, QWidget *parent)
	: classroom(clroom),tcpclient(tcpc), m_method(method),QWidget(parent)
{
	attendance = new ATTEND;
	already_attend = new QVector<ATTEND>;
	isLeisure = true;//��ʼ����״̬
	iscomplete = false;//
	isrecvdata = false;
	//ui.setupUi(this);
	SetupUi();

	connect(buttok, SIGNAL(clicked()), this, SLOT(attend()));
	connect(buttAttList, SIGNAL(clicked()), this, SLOT(attendList()));
	QObject::connect(this, &AttendanceM::chang, this, &AttendanceM::Changeui);
	QObject::connect(tcpclient->GetSock(), &QTcpSocket::readyRead, this, &AttendanceM::RecvHuiZhiPro);
	QObject::connect(&deal, &CDeal::RecvAttenance, this, &AttendanceM::slotSign);

	
	_beginthread(SendOrderPro, 0, this);
	
}

AttendanceM::~AttendanceM()
{

}
void AttendanceM::SendOrderPro(PVOID another)
{
	AttendanceM * attend = (AttendanceM *)another;

	emit attend->chang();

	while (1)
	{
		//Sleep(60000 * 5);//3����
		Sleep(10000);//10s
		if (attend->isLeisure)//�Ƿ����
		{
			//���������������������
			SendReQuest(attend);

			//�ȴ���ִ
			int waittime = 0;
			while (!attend->isrecvdata)
			{
				Sleep(1000 * 5);
				waittime++;

				if (waittime == 12 * 2)//�������2����������ȴ�
					break;
				
			}
			if (attend->isrecvdata)//�յ���ִ
			{
				attend->isrecvdata = false;

				int currtime = QDateTime::currentDateTime().toTime_t();//��ǰʱ���ʱ���
				//�����������е�ѧ������������0   ����  �ÿ�����Ŀû�п��ڹ������ܿ���(���ڹ���)  ���ҿ�ʼʱ�����Ҫ���ڵ�ǰʱ��
				if (attend->attendance->Stuends.size() != 0 && !attend->AttendFiltration() && currtime < attend->attendance->starttime)
				{
					attend->isLeisure = false;//��״̬���÷ǿ���
					
					emit attend->chang();


					//׼����ʼ����()
					AttendPro(attend);
				}
				
			}
			
		}
		else//������ǿ���״̬�����ж��Ƿ������
		{
			if (attend->iscomplete)
			{
				//���Ϳ��ڽ������

				SendResult(attend);

				//��յ�ǰ������Ŀ����
				attend->attendance->signcount = 0;
				attend->attendance->Stuends.clear();//��տ���������ֻ�������ڱ�źͿ���ʱ��

				attend->already_attend->append(*attend->attendance);//��ֻ�п��ڱ�źͿ���ʱ�εĿ�����Ŀ�����ѿ����б��Ա������ͬ������Ŀ


				attend->attendance->attendanceid = "0";//Ȼ�����ʣ�µ�������Ϣ
				attend->attendance->starttime = 0;
				attend->attendance->endtiem = 0;
				
				

				attend->isLeisure = true;//��״̬���ÿ���
				attend->iscomplete = false;

				emit attend->chang();
				

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

	//������Ŀ���
	QByteArray tempQBattid = another->attendance->attendanceid.toLatin1();
	char * attid = tempQBattid.data();

	memcpy(&ptrSendData[19], attid, 8);

	//��¼��
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

	
	deal.FenLiZhen(netbao, recvlen, attendance);//���ô���ģ�齫���ܵ������ݷ�֡ 


}
void AttendanceM::attend()
{
#if 1
	QString input = editinput->text();

	for (auto it = attendance->Stuends.begin(); it != attendance->Stuends.end(); it++)
	{
		if (m_method == 0)//ѧ��ǩ��
		{
			if (input == it->StuId)
			{
				it->StuSign = true;
				attendance->signcount++;//ǩ��һ���ˣ���¼һ��
			}
		}
		else//ָ��ǩ��
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
	isrecvdata = true;//�յ�����


}

void AttendanceM::attendList()
{
	//attlist attendtable(&attendance->Stuends);
	attlist logindialog(&attendance->Stuends);

	logindialog.setModal(true);//����ģ̬����

	logindialog.exec();
		/*if (logindialog.exec() != QDialog::Accepted)
		{

		}*/
}

void AttendanceM::AttendPro(AttendanceM * another)
{
	//	

	//
	//Sleep(another->attendance->starttime - currtime);//��ʱ�ȴ�����ʼʱ��

	while (another->attendance->starttime > QDateTime::currentDateTime().toTime_t())
	{

	}

	emit another->chang();
	while (1)
	{
		Sleep(5000);
		int currtime = QDateTime::currentDateTime().toTime_t();//��ǰʱ���ʱ���
		if ( currtime > another->attendance->endtiem || another->attendance->signcount == another->attendance->Stuends.size())//������ʱ��������ߣ��Ѿ�ȫ��ǩ������Ժ�
		{
			another->iscomplete = true;//��ɿ���
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

void AttendanceM::Changeui()
{
	if (!isLeisure)
	{
		this->setWindowTitle(tr("Attendance Machine Simulator Attend"));
		if (this->attendance->starttime <= QDateTime::currentDateTime().toTime_t())//��ʽ��ʼ����
		{
			this->editinput->setVisible(true);
			this->label1->setVisible(true);
			this->buttok->setVisible(true);
			this->buttAttList->setVisible(true);
			
		}

		this->labelAttid->setVisible(true);
		this->labelAttname->setVisible(true);
		this->labelAttstarttime->setVisible(true);
		this->labelAttendtime->setVisible(true);
		this->edidAttid->setVisible(true);
		this->edidAttname->setVisible(true);
		this->edidAttstarttime->setVisible(true);
		this->edidAttendtime->setVisible(true);

		this->edidAttid->setText(attendance->attendanceid);
		this->edidAttname->setText("N/A");

		QDateTime tempdatetime(QDateTime::fromTime_t(attendance->starttime));
		this->edidAttstarttime->setText(tempdatetime.toString("yyyy-MM-dd HH:mm"));
		tempdatetime = (QDateTime::fromTime_t(attendance->endtiem));
		this->edidAttendtime->setText(tempdatetime.toString("yyyy-MM-dd HH:mm"));

	}
	else
	{
		this->setWindowTitle(tr("Attendance Machine Simulator"));
		this->editinput->setVisible(false);
		this->label1->setVisible(false);
		this->buttok->setVisible(false);
		this->buttAttList->setVisible(false);

		this->labelAttid->setVisible(false);
		this->labelAttname->setVisible(false);
		this->labelAttstarttime->setVisible(false);
		this->labelAttendtime->setVisible(false);
		this->edidAttid->setVisible(false);
		this->edidAttname->setVisible(false);
		this->edidAttstarttime->setVisible(false);
		this->edidAttendtime->setVisible(false);
	}
}

void AttendanceM::SetupUi()
{
	this->setWindowTitle(tr("Attendance Machine Simulator"));
	this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint/* | Qt::WindowMaximizeButtonHint*//*| Qt::Widget | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint*/);//�رպ���С����ť
																																																					  //this->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);

	this->setFixedSize(QSize(500, 400));

	//��ʾ��Ϣ
	labelAttid = new QLabel(tr("Attendance id"));
	labelAttname = new QLabel(tr("Attendance name"));
	labelAttstarttime = new QLabel(tr("Attendance starttime"));
	labelAttendtime = new QLabel(tr("Attendance endtime"));
	edidAttid = new QLabel("cs100101");
	edidAttname = new QLabel("cs100101");
	edidAttstarttime = new QLabel("2018-2-7 12:00");
	edidAttendtime = new QLabel("2018-2-7 13:00");

	labelAttid->setAlignment(Qt::AlignVCenter);
	labelAttname->setAlignment(Qt::AlignVCenter);
	labelAttstarttime->setAlignment(Qt::AlignVCenter);
	labelAttendtime->setAlignment(Qt::AlignVCenter);
	edidAttid->setAlignment(Qt::AlignVCenter);
	edidAttname->setAlignment(Qt::AlignVCenter);
	edidAttstarttime->setAlignment(Qt::AlignVCenter);
	edidAttendtime->setAlignment(Qt::AlignVCenter);


	editinput = new QLineEdit;
	
	if (m_method == 0)//ѧ��ǩ��
	{
		label1 = new QLabel(tr("StuId"));
	}
	else
	{
		label1 = new QLabel(tr("Finger"));
	}
	buttok = new QPushButton(tr("submit"));
	buttAttList = new QPushButton(tr("Attendance List"));

	QHBoxLayout * layout_top1 = new QHBoxLayout;
	layout_top1->addStretch(2);
	layout_top1->addWidget(labelAttid);	
	layout_top1->addWidget(edidAttid);
	layout_top1->addStretch(1);
	layout_top1->addWidget(labelAttname);
	layout_top1->addWidget(edidAttname);
	layout_top1->addStretch(2);

	QHBoxLayout * layout_top2 = new QHBoxLayout;
	layout_top2->addStretch(2);
	layout_top2->addWidget(labelAttstarttime);
	layout_top2->addWidget(edidAttstarttime);
	layout_top2->addStretch(1);
	layout_top2->addWidget(labelAttendtime);
	layout_top2->addWidget(edidAttendtime);	
	layout_top2->addStretch(2);



	QHBoxLayout * layout_border = new QHBoxLayout;

	layout_border->addStretch(1);
	layout_border->addWidget(label1);
	layout_border->addWidget(editinput);
	layout_border->addWidget(buttok);
	layout_border->addWidget(buttAttList);
	layout_border->addStretch(1);

	QVBoxLayout * layout_main = new QVBoxLayout;
	layout_main->addStretch(2);
	layout_main->addLayout(layout_top1);
	layout_main->addLayout(layout_top2);
	layout_main->addStretch(1);
	layout_main->addLayout(layout_border);
	layout_main->addStretch(2);


	this->setLayout(layout_main);



}
