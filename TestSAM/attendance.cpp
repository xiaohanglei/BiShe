#include "attendance.h"
#include <QPushButton>
AttendanceM	::AttendanceM(TcpClient * tcpc, int method, QWidget *parent)
	:tcpclient(tcpc), m_method(method),QWidget(parent)
{
	Stuends = new QVector<STUDENT>;
	isLeisure = true;//��ʼ����״̬
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
		Sleep(60000 * 5);//����
		if (isLeisure)//�Ƿ����
		{
			//���������������������

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
