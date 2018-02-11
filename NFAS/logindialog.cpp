#include "logindialog.hpp"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>

LoginDialog::LoginDialog(DataManager* dm,QWidget * parent) : QDialog(parent) 
{
	dataManager = dm;
	setupUi();

	
	//将登录按钮和槽函数进行连接
	connect(login, SIGNAL(clicked()), this, SLOT(Login()));
	
}

LoginDialog::~LoginDialog() 
{
	
}

void LoginDialog::Login()
{
	QString uid = userid->text().trimmed();
	QString psd = userpassword->text();

	//判断是否有输入
	if (uid.length() == 0) 
	{
		QMessageBox::information(0, tr("user login"), tr("user id is required"), QMessageBox::Ok);
		QDialog::rejected();
		return;
	}
	if (psd.length() == 0) 
	{
		QMessageBox::information(0, tr("user login"), tr("user password is required"), QMessageBox::Ok);
		QDialog::rejected();
		return;
	}

	//判断是否输入正确
	if (dataManager->UserLogin(User(uid, psd, 0)))
	{
		//匹配正确以后，将该用户所提交的考勤结果表从数据库中加载到程序中(如果是管理员，则加载全部)
		dataManager->updateResult(dataManager->GetCurrentUser());//
		QDialog::accept();//正确退出该对话框
	}
	else //不匹配
	{
		userid->setText("");
		userpassword->setText("");

#ifdef SERVER
		QMessageBox::information(0, tr("user login"), tr("user id or user password is not match or not is Admini"), QMessageBox::Ok);
#else
		QMessageBox::information(0, tr("user login"), tr("user id or user password is not match"), QMessageBox::Ok);
#endif

		
		QDialog::rejected();
	}
}

void LoginDialog::setupUi()//界面函数
{
	this->setWindowTitle(tr("user login"));//窗口标题
	this->setWindowFlags(Qt::WindowCloseButtonHint);
	this->setFixedSize(QSize(600, 400));

	QImage image;
	image.load(":/NFAS/Resources/logo.png");
	QLabel* logo = new QLabel;
	logo->setPixmap(QPixmap::fromImage(image));
	QHBoxLayout* logolayout = new QHBoxLayout;//左右布局
	logolayout->addStretch(1);//左占位符
	logolayout->addWidget(logo);//图标
	logolayout->addStretch(1);//右占位符

#ifdef SERVER	
	QLabel* sysinfo = new QLabel(tr("Network fingerprint attendance system Server"));//标题,网络指纹考勤系统
#else
	QLabel* sysinfo = new QLabel(tr("Network fingerprint attendance system "));//标题,网络指纹考勤系统
#endif
	QFont ft;
	ft.setPointSize(20);//设置字体大小	
	sysinfo->setFont(ft);

	QHBoxLayout* sysinfolayout = new QHBoxLayout;//左右布局器
	sysinfolayout->addStretch(1);
	sysinfolayout->addWidget(sysinfo);
	sysinfolayout->addStretch(1);

	//添加标签和文本框控件
	QLabel* labeluserid = new QLabel(tr("user id"));
	userid = new QLineEdit("admini");//默认值
	userid->setMaxLength(6);
	QLabel* labelpsd = new QLabel(tr("user password"));
	userpassword = new QLineEdit("586926");
	userpassword->setEchoMode(QLineEdit::Password);//设置密码属性
	login = new QPushButton(tr("login"));//登录按钮
	login->setMinimumWidth(100);

	//利用布局器进行组合布局
	QVBoxLayout* main_layout = new QVBoxLayout;//整体上下布局器
	main_layout->addStretch(1);
	main_layout->addLayout(logolayout);//显示信息布局器
	main_layout->addStretch(1);
	main_layout->addLayout(sysinfolayout);//添加标题
	main_layout->addStretch(2);
	QHBoxLayout* idlayout = new QHBoxLayout;
	idlayout->addStretch(1);
	idlayout->addWidget(labeluserid);
	idlayout->addWidget(userid);
	idlayout->addStretch(1);
	QHBoxLayout* psdlayout = new QHBoxLayout;
	psdlayout->addStretch(1);
	psdlayout->addWidget(labelpsd);
	psdlayout->addWidget(userpassword);
	psdlayout->addStretch(1);
	QHBoxLayout* buttonlayout = new QHBoxLayout;
	
	buttonlayout->addStretch(1);
	buttonlayout->addWidget(login);
	buttonlayout->addStretch(1);
	main_layout->addLayout(idlayout);
	main_layout->addLayout(psdlayout);
	main_layout->addStretch(1);
	main_layout->addLayout(buttonlayout);
	main_layout->addStretch(2);
	this->setLayout(main_layout);
}
