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

//#include <QtWidgets/QApplication>
#include <QDesktopWidget>

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

void NFAS::ExeMingLingProc(LPVOID another)
{
	DataManager * dm = (DataManager *)another;

	NETBAO xinxibao ;

	while (1)
	{
		Sleep(1000);
	
		EnterCriticalSection(dm->GetCriNetBao());//线程锁
		
		if (dm->GetNetBao()->isEmpty())//当命令队列为空时，跳过本次循环
		{
			LeaveCriticalSection(dm->GetCriNetBao());
			continue;
		}

		xinxibao = dm->GetNetBao()->front();//取出命令队列中的首条命令
		dm->GetNetBao()->pop_front();//取出要执行的命令以后，从队列中删除该条命令		

		LeaveCriticalSection(dm->GetCriNetBao());

		//检查设备列表中有无该设备
		QString tempQS;
		tempQS = QString(QLatin1String((char * )xinxibao.sostation));

		if ( !dm->FindDevic(tempQS))
			continue;

		switch (xinxibao.lxm)
		{
		case 0x11://待考勤学生名单命令
			//Deal::Deal_Attendance_MingLing(xinxibao);
			Deal::Deal_Attendance_HuiZhi(xinxibao.sostation, xinxibao.sock,dm);//不用解析命令，直接回执
			break;
		case 0x12://推送考勤记录表命令
			Deal::Deal_Result_MingLing(xinxibao);
			break;

		}		
	}

}

//选项卡切换
void NFAS::UpdateTab(int index)

{
	//只有当前用户为管理员则可以显示所有选项卡，切换
	if (dataManager->GetCurrentUser().GetIdentify() == 0)
	{
		auto widget = main_tab->currentWidget();
		switch (index)
		{
		case 0:
			((AcademicClassStudentTabWidget*)widget)->UpdateTab();//学院、班级、学生选项卡
			break;
		case 1:
			((AttendanceTabWidget*)widget)->UpdateTab();//考勤信息
			break;
		case 2:
			((ResultTabWidget*)widget)->UpdateTab();//考勤结果
			break;
		case 3:
			((UserTabWidget*)widget)->UpdateTab(); //用户管理
			break;
		default:
			break;
		}
	}
}

void NFAS::setupUi()
{
	this->setWindowTitle(tr("Network fingerprint attendance system"));
	this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint/*| Qt::Widget | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint*/);//关闭和最小化按钮
	//this->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);

	//this->setFixedSize(QSize(1440,900));

	QRect a = (QApplication::desktop())->availableGeometry();
	this->setGeometry(0,30,1440,900);
	//this->setFixedSize(QSize(a.width(), a.height()-100));
	//this->showFullScreen();//全屏显示
	this->showMaximized();	

	main_tab = new QTabWidget;

	//添加选项卡
	if (dataManager->GetCurrentUser().GetIdentify() == 0) //只有当前用户为管理员时，才添加所有选项卡，否则只添加考勤结果选项卡
	{
		main_tab->addTab(new AttendanceTabWidget(dataManager), tr("attendance information"));//考勤信息
			
	}
	main_tab->addTab(new ResultTabWidget(dataManager), tr("result information"));//考勤结果

	if (dataManager->GetCurrentUser().GetIdentify() == 0) 
	{
		main_tab->addTab(new AcademicClassStudentTabWidget(dataManager), tr("Academic class student"));//学院、班级、学生	
		main_tab->addTab(new UserTabWidget(dataManager), tr("user information"));//用户管理
		main_tab->addTab(new devicewidget(dataManager), tr("Device information"));//设备管理
	}
	
	//设置布局
	QVBoxLayout* main_layout = new QVBoxLayout;
	main_layout->addWidget(main_tab);
	this->setLayout(main_layout);

	//开启线程
	_beginthread(TcpServer::RecvClientProc, 0, dataManager);//接收客户端连接的线程

	_beginthread(ExeMingLingProc, 0, dataManager);//处理考勤设备发来的命令


//测试
	//UCHAR station[7] = "10-211";
	//Deal::Deal_Attendance_HuiZhi(station, 0, dataManager);

	//
	
}
