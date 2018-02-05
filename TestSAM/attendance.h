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

#define ATTENDANCEMAX 100//单次考勤最大数量
#define MAX_BUFF 1024 * 1024//分帧缓冲区大小
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
	bool isLeisure;//是否空闲
	bool iscomplete;//是否完成
	bool isrecvdata;//是否收到回执

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
