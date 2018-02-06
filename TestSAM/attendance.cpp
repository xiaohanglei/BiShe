#include "attendance.h"
#include <QPushButton>
#include <windows.h>
#include <process.h>
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

				//�����������е�ѧ������������0   ����  �ÿ�����Ŀû�п��ڹ������ܿ���(���ڹ���)
				if (attend->attendance->Stuends.size() != 0 && !attend->AttendFiltration())
				{
					attend->isLeisure = false;//��״̬���÷ǿ���
					
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

void AttendanceM::AttendPro(AttendanceM * another)
{
	//

	int currtime = QDateTime::currentDateTime().toTime_t();//��ǰʱ���ʱ���

	//
	Sleep(another->attendance->starttime - currtime);//��ʱ�ȴ�����ʼʱ��



	while (1)
	{
		Sleep(5000);
		currtime = QDateTime::currentDateTime().toTime_t();//��ǰʱ���ʱ���
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

void AttendanceM::SetupUi()
{
	this->setWindowTitle(tr("Attendance Machine Simulator"));
	this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint/* | Qt::WindowMaximizeButtonHint*//*| Qt::Widget | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint*/);//�رպ���С����ť
																																																					  //this->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);

	this->setFixedSize(QSize(400, 200));



	editinput = new QLineEdit;
	QLabel * label1;
	if (m_method == 0)//ѧ��ǩ��
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
