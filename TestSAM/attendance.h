#ifndef ATTENDANCE_H
#define ATTENDANCE_H

#include <QWidget>
//#include "ui_attendance.h"
#include "tcpclient.hpp"

#include <QLineEdit>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QVector>

struct STUDENT
{
	QString StuId;
	QString StuFinger;
	bool StuSign;
};

class AttendanceM : public QWidget
{
	Q_OBJECT

public:
	AttendanceM(TcpClient *tcpc,int method,QWidget *parent = 0);
	~AttendanceM();
	bool isLeisure;//�Ƿ����
	bool iscomplete;//�Ƿ����
	bool isrecvdata;//�Ƿ��յ���ִ

	QVector<STUDENT> * GetStudent()
	{
		return Stuends;
	}

	 void SendOrderPro(PVOID another);
	 void RecvHuiZhiPro();


public slots:

void slotSign();

	
private:
	void SetupUi();
	TcpClient * tcpclient;
	QLineEdit * editinput;
	QPushButton * buttok;
	int m_method;

	QVector<STUDENT> *Stuends;

	
};

#endif // ATTENDANCE_H
