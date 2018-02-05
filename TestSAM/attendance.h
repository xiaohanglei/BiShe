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

class AttendanceM : public QWidget
{
	Q_OBJECT

public:
	AttendanceM(QString classroom,TcpClient *tcpc,int method,QWidget *parent = 0);
	~AttendanceM();
	bool isLeisure;//�Ƿ����
	bool iscomplete;//�Ƿ����
	bool isrecvdata;//�Ƿ��յ���ִ

	static void SendOrderPro(PVOID another);
	 
	 void RecvHuiZhiPro();

	 static void Attend(AttendanceM * another);//����
public slots:


void slotSign();
static void SendReQuest(AttendanceM* another);

	
private:
	void SetupUi();
public:
	TcpClient * tcpclient;
	QLineEdit * editinput;
	QPushButton * buttok;
	int m_method;
	CDeal deal;

	QString classroom;

	ATTEND * attendance;

	
};


#endif // ATTENDANCE_H
