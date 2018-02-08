#include "nfas.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTabWidget>

//#include "studenttabwidget.hpp"
#include "attendancetabwidget.hpp"
#include "resulttabwidget.hpp"
#include "usertabwidget.hpp"
#include "academicclassstudenttabwidget.hpp"
#include "devicewidget.hpp"
#include <QMessageBox>
#include <QTcpSocket>

//#include <QtWidgets/QApplication>
#include <QDesktopWidget>

NFAS::NFAS(DataManager *dm,QWidget *parent)
	: QDialog(parent)
{
	dataManager = dm;
	IsNetWork = false;;//
	setupUi();
	connect(main_tab, SIGNAL(currentChanged(int)), this, SLOT(UpdateTab(int)));

	

	//connect(testdeal, SIGNAL(&Deal::newPaper(QString)), this, SLOT(slotstestdeal(QString)));

	//testdeal->sendtest();	

}

NFAS::~NFAS()
{
	
}

void NFAS::ExeMingLingProc(LPVOID another)
{
	DataManager * dm = (DataManager *)another;

	
	NETBAO xinxibao ;


	while (1)
	{
		Sleep(10000);
	
		EnterCriticalSection(dm->GetCriNetBao());//�߳���
		
		if (dm->GetNetBao()->isEmpty())//���������Ϊ��ʱ����������ѭ��
		{
			LeaveCriticalSection(dm->GetCriNetBao());
			continue;
		}

		xinxibao = dm->GetNetBao()->first();//ȡ����������е���������
		dm->GetNetBao()->removeFirst();//ȡ��Ҫִ�е������Ժ󣬴Ӷ�����ɾ����������		

		LeaveCriticalSection(dm->GetCriNetBao());

		

		//����豸�б������޸��豸
		QString tempQS;
		tempQS = QString(QLatin1String((char * )xinxibao.sostation));

		if ( !dm->FindDevic(tempQS))
			continue;

		switch (xinxibao.lxm)
		{
		case 0x11://������ѧ����������
			//Deal::Deal_Attendance_MingLing(xinxibao);
			Deal::Deal_Attendance_HuiZhi(xinxibao.sostation, xinxibao.sock,dm);//���ý������ֱ�ӻ�ִ
			break;
		case 0x12://���Ϳ��ڼ�¼������
			Deal::Deal_Result_MingLing(xinxibao,dm);
			break;

		}		
	}

}

void NFAS::closeEvent(QCloseEvent * event)
{
	QMessageBox::StandardButton button;
	button = QMessageBox::question(this, tr("Exit"), QString(tr("Affirm Exit")), QMessageBox::Yes | QMessageBox::No);
	if (button == QMessageBox::No)
	{
		event->ignore(); // �����˳��źţ������������  
	}
	else if (button == QMessageBox::Yes)
	{
		if(IsNetWork)//�������������ͨѶ���ܵĿͻ���ʱ������������
			dataManager->IsServerOnline(3);
		event->accept(); // �����˳��źţ������˳�  
	}
}

//ѡ��л�
void NFAS::UpdateTab(int index)

{
	//ֻ�е�ǰ�û�Ϊ����Ա�������ʾ����ѡ����л�
	if (dataManager->GetCurrentUser().GetIdentify() == 0)
	{
		auto widget = main_tab->currentWidget();
		switch (index)
		{
		case 0:
			((AttendanceTabWidget*)widget)->UpdateTab();//������Ϣ
			break;		
		case 1:
			((ResultTabWidget*)widget)->UpdateTab();//���ڽ��
			break;
		case 2:
			((AcademicClassStudentTabWidget*)widget)->UpdateTab();//ѧԺ���༶��ѧ��ѡ�
			break;
		case 3:
			((UserTabWidget*)widget)->UpdateTab(); //�û�����
			break;
		case 4:
			((devicewidget*)widget)->UpdateTab(); //�豸��Ϣ
			break;
		default:
			break;
		}
	}
}

void NFAS::setupUi()
{
	this->setWindowTitle(tr("Network fingerprint attendance system"));
	this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint/*| Qt::Widget | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint*/);//�رպ���С����ť
	//this->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);

	//this->setFixedSize(QSize(1440,900));

	QRect a = (QApplication::desktop())->availableGeometry();
	this->setGeometry(400,320,1440,900);
	//this->setFixedSize(QSize(a.width(), a.height()-100));
	//this->showFullScreen();//ȫ����ʾ
	//this->showMaximized();	

	main_tab = new QTabWidget;

	//���ѡ�
	if (dataManager->GetCurrentUser().GetIdentify() == 0) //ֻ�е�ǰ�û�Ϊ����Աʱ�����������ѡ�������ֻ��ӿ��ڽ��ѡ�
	{
		main_tab->addTab(new AttendanceTabWidget(dataManager), tr("attendance information"));//������Ϣ
			
	}
	ResultTabWidget * temp = new ResultTabWidget(dataManager);
	main_tab->addTab(temp, tr("result information"));//���ڽ��
	//connect(testdeal, SIGNAL(&Deal::newPaper(QString)), temp, SLOT(&ResultTabWidget::slotstestdeal(QString)));

	if (dataManager->GetCurrentUser().GetIdentify() == 0) 
	{
		main_tab->addTab(new AcademicClassStudentTabWidget(dataManager), tr("Academic class student"));//ѧԺ���༶��ѧ��	
		main_tab->addTab(new UserTabWidget(dataManager), tr("user information"));//�û�����
		main_tab->addTab(new devicewidget(dataManager), tr("Device information"));//�豸����
	}
	
	//���ò���
	QVBoxLayout* main_layout = new QVBoxLayout;
	main_layout->addWidget(main_tab);
	this->setLayout(main_layout);


	QString testip;//������߷�������ip
	if (dataManager->GetCurrentUser().GetIdentify() == 0 && !dataManager->IsServerOnline(1,testip))//ֻ�е�ǰ�û�Ϊ����Ա��û�о���ͨѶ���ܵĹ���ͻ�������ʱ�����ܿ�������ͨѶ����
	{
		//�������߱��
		dataManager->IsServerOnline(2);
		IsNetWork = true;
		this->setWindowTitle(tr("Network fingerprint attendance system online"));

		dataManager->GetTcp()->StartServer();//�����������
		//�����߳�
		_beginthread(TcpServer::RecvClientProc, 0, dataManager);//���տͻ������ӵ��߳�
		_beginthread(ExeMingLingProc, 0, dataManager);//�������豸����������
	}
	else//����Ѿ��з������ߣ������ж����ߵķ������Ƿ�������������ͨ��״̬
	{
		testip = "127.0.0.1";//������߷�������ip
		if (testip.length() != 0)//��û�л�ȡ����������ip���򽫵�ǰϵͳ����Ϊ������
		{

			QTcpSocket testNet;
			testNet.connectToHost(testip, 9696);

			if (testNet.waitForConnected(10000))//���õȴ�����ʱ��Ϊ 10s
			{
				//���ӳɹ�����֤����ǰ������������

				testNet.disconnectFromHost();//�Ͽ�����
				testNet.waitForDisconnected();
			}
			else//Ҫ������ʧ�ܣ��򽫵�ǰϵͳ����Ϊ������
			{
				testNet.disconnectFromHost();//�Ͽ�����
				testNet.waitForDisconnected();


				//�������߱��
				dataManager->IsServerOnline(2);
				IsNetWork = true;
				this->setWindowTitle(tr("Network fingerprint attendance system online"));

				dataManager->GetTcp()->StartServer();//�����������
													 //�����߳�
				_beginthread(TcpServer::RecvClientProc, 0, dataManager);//���տͻ������ӵ��߳�
				_beginthread(ExeMingLingProc, 0, dataManager);//�������豸����������

			}
		}
		else
		{
			
			//�������߱��
			dataManager->IsServerOnline(2);
			IsNetWork = true;
			this->setWindowTitle(tr("Network fingerprint attendance system online"));

			dataManager->GetTcp()->StartServer();//�����������
												 //�����߳�
			_beginthread(TcpServer::RecvClientProc, 0, dataManager);//���տͻ������ӵ��߳�
			_beginthread(ExeMingLingProc, 0, dataManager);//�������豸����������
		}

	}

//����
	//UCHAR station[7] = "10-211";
	//Deal::Deal_Attendance_HuiZhi(station, 0, dataManager);

	//
	
}
