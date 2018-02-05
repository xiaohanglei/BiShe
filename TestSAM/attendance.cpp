#include "attendance.h"
#include <QPushButton>
#include <windows.h>
#include <process.h>
AttendanceM	::AttendanceM(QString clroom, TcpClient * tcpc, int method, QWidget *parent)
	: classroom(clroom),tcpclient(tcpc), m_method(method),QWidget(parent)
{
	attendance = new ATTEND;
	isLeisure = true;//��ʼ����״̬
	iscomplete = false;//
	isrecvdata = false;
	//ui.setupUi(this);
	SetupUi();

	connect(buttok, SIGNAL(clicked()), this, SLOT(SendReQuest()));
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

	while (1)
	{
		Sleep(60000 * 5);//����
		if (attend->isLeisure)//�Ƿ����
		{
			//���������������������
			SendReQuest(attend);

			//�ȴ���ִ
			int waittime = 0;
			while (!attend->isrecvdata)
			{
				Sleep(1000 * 2);
				waittime++;

				if (waittime == 12 * 2)//�������2����������ȴ�
					break;
				
			}
			if (attend->isrecvdata)//�յ���ִ
			{
				attend->isLeisure = false;//��״̬���÷ǿ���
				attend->isrecvdata = false;

				if (attend->attendance->Stuends.size() != 0)
				{
					//׼����ʼ����()
					Attend(NULL);
				}
				
			}
			
		}
		else//������ǿ���״̬�����ж��Ƿ������
		{
			if (attend->iscomplete)
			{
				//���Ϳ��ڽ������
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

void AttendanceM::slotSign()
{
	//
	isrecvdata = true;//�յ�����

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

void AttendanceM::Attend(AttendanceM * another)
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
