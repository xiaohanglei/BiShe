#include "nfas.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTabWidget>
//#include "studenttabwidget.hpp"
#include "attendancetabwidget.hpp"
#include "resulttabwidget.hpp"
#include "usertabwidget.hpp"
#include "academicclassstudenttabwidget.hpp"


NFAS::NFAS(DataManager *dm,QWidget *parent)
	: QDialog(parent)
{
	dataManager = dm;
	setupUi();

	connect(main_tab, SIGNAL(currentChanged(int)), this, SLOT(UpdateTab(int)));
}

NFAS::~NFAS()
{

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
			((AcademicClassStudentTabWidget*)widget)->UpdateTab();//ѧԺ���༶��ѧ��ѡ�
			break;
		case 1:
			((AttendanceTabWidget*)widget)->UpdateTab();//������Ϣ
			break;
		case 2:
			((ResultTabWidget*)widget)->UpdateTab();//���ڽ��
			break;
		case 3:
			((UserTabWidget*)widget)->UpdateTab(); //�û�����
			break;
		default:
			break;
		}
	}
}

void NFAS::setupUi()
{
	this->setWindowTitle(tr("Network fingerprint attendance system"));
	this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);//�رպ���С����ť
	this->setFixedSize(QSize(1200, 850));
	main_tab = new QTabWidget;

	//���ѡ�
	if (dataManager->GetCurrentUser().GetIdentify() == 0) //ֻ�е�ǰ�û�Ϊ����Աʱ�����������ѡ�������ֻ��ӿ��ڽ��ѡ�
	{
		main_tab->addTab(new AcademicClassStudentTabWidget(dataManager), tr("Academic class student"));//ѧԺ���༶��ѧ��
		main_tab->addTab(new AttendanceTabWidget(dataManager), tr("attendance information"));//������Ϣ
	}

	main_tab->addTab(new ResultTabWidget(dataManager), tr("result information"));//���ڽ��

	if (dataManager->GetCurrentUser().GetIdentify() == 0) 
	{
		main_tab->addTab(new UserTabWidget(dataManager), tr("user information"));//�û�����
	}
	//���ò���
	QVBoxLayout* main_layout = new QVBoxLayout;
	main_layout->addWidget(main_tab);
	this->setLayout(main_layout);

	//�����߳�
	_beginthread(TcpServer::RecvClientProc, 0, dataManager->tcp);
	
}
