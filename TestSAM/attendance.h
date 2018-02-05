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
#include <deal.h>

#define ATTENDANCEMAX 100//���ο����������
#define MAX_BUFF 1024 * 1024//��֡��������С
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
	AttendanceM(QString classroom,TcpClient *tcpc,int method,QWidget *parent = 0);
	~AttendanceM();
	bool isLeisure;//�Ƿ����
	bool iscomplete;//�Ƿ����
	bool isrecvdata;//�Ƿ��յ���ִ

	QVector<STUDENT> * GetStudent()
	{
		return Stuends;
	}

	 void SendOrderPro(PVOID another);
	 void FenLiZhen(UCHAR * recvbuff, int len);
	 
	 void RecvHuiZhiPro();


public slots:

void slotSign();
void SendReQuest();

	
private:
	void SetupUi();
	TcpClient * tcpclient;
	QLineEdit * editinput;
	QPushButton * buttok;
	int m_method;

	QString classroom;

	QVector<STUDENT> *Stuends;

	
};

#endif // ATTENDANCE_H
