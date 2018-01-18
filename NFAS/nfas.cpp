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
	this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);//关闭和最小化按钮
	this->setFixedSize(QSize(1200, 850));
	main_tab = new QTabWidget;

	//添加选项卡
	if (dataManager->GetCurrentUser().GetIdentify() == 0) //只有当前用户为管理员时，才添加所有选项卡，否则只添加考勤结果选项卡
	{
		main_tab->addTab(new AcademicClassStudentTabWidget(dataManager), tr("Academic class student"));//学院、班级、学生
		main_tab->addTab(new AttendanceTabWidget(dataManager), tr("attendance information"));//考勤信息
	}

	main_tab->addTab(new ResultTabWidget(dataManager), tr("result information"));//考勤结果

	if (dataManager->GetCurrentUser().GetIdentify() == 0) 
	{
		main_tab->addTab(new UserTabWidget(dataManager), tr("user information"));//用户管理
	}
	//设置布局
	QVBoxLayout* main_layout = new QVBoxLayout;
	main_layout->addWidget(main_tab);
	this->setLayout(main_layout);

	//开启线程
	_beginthread(TcpServer::RecvClientProc, 0, dataManager->tcp);
	
}
