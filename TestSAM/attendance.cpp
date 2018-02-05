#include "attendance.h"
#include <QPushButton>
AttendanceM	::AttendanceM(QString clroom, TcpClient * tcpc, int method, QWidget *parent)
	: classroom(clroom),tcpclient(tcpc), m_method(method),QWidget(parent)
{
	Stuends = new QVector<STUDENT>;
	isLeisure = true;//��ʼ����״̬
	iscomplete = false;//
	//ui.setupUi(this);
	SetupUi();

	connect(buttok, SIGNAL(clicked()), this, SLOT(SendReQuest()));
	QObject::connect(tcpclient->GetSock(), &QTcpSocket::readyRead, this, &AttendanceM::RecvHuiZhiPro);
}

AttendanceM::~AttendanceM()
{

}
void AttendanceM::SendOrderPro(PVOID another)
{
	while (1)
	{
		Sleep(60000 * 5);//����
		if (isLeisure)//�Ƿ����
		{
			//���������������������
			SendReQuest();

			//�ȴ���ִ
			int waittime = 0;
			while (!isrecvdata)
			{
				Sleep(1000 * 5);
				waittime++;

				if (waittime == 5 * 12 * 2)//�������2����������ȴ�
					break;
				
			}
			if (isrecvdata)//�յ���ִ
			{
				isLeisure = false;//��״̬���÷ǿ���

				//׼����ʼ����
			}
			
		}
		else//������ǿ���״̬�����ж��Ƿ������
		{
			if (iscomplete)
			{
				//���Ϳ��ڽ������

				//�ȴ���ִ

			}
			continue;
		}
	}
}
void AttendanceM::FenLiZhen(UCHAR * recvbuff, int len)
{

}
void AttendanceM::SendReQuest()
{
	UCHAR * ptrSendData = new UCHAR[MAX_BUFF];
	int sendlen = 0;
	ptrSendData[4] = 0x8f;

	char mdzm[7] = "Server";
	char *sozm = nullptr;
	QByteArray tempQB = classroom.toLatin1();
	sozm = tempQB.data();

	memcpy(&ptrSendData[5], mdzm, 6);
	memcpy(&ptrSendData[11], sozm, 6);
	ptrSendData[17] = 0x01;
	ptrSendData[18] = 0x11;

	sendlen += 14;

	ptrSendData[sendlen + 5] = 0xff;
	sendlen += 1;
	memcpy(&ptrSendData[0], &sendlen, 4);

	tcpclient->GetSock()->write(((char *)ptrSendData),sendlen + 5);


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
	/*if (netbao[0] == 0xff)
	{
		QMessageBox::information(0, tr("Success"), tr("Recv Data"), QMessageBox::Ok);
	}*/
	Deal deal;
	deal.FenLiZhen(netbao, recvlen);


}
void AttendanceM::slotSign()
{
	QString input = editinput->text();

	for (auto it = Stuends->begin(); it != Stuends->end(); it++)
	{
		if (m_method == 0)//ѧ��ǩ��
		{
			if (input == it->StuId)
			{
				it->StuSign = true;
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
