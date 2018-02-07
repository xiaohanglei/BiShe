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

class AttendanceM : public QWidget
{
	Q_OBJECT

public:
	AttendanceM(QString classroom,TcpClient *tcpc,int method,QWidget *parent = 0);
	~AttendanceM();
	bool isLeisure;//是否空闲
	bool iscomplete;//是否完成
	bool isrecvdata;//是否收到回执

	static void SendOrderPro(PVOID another);
	static void SendReQuest(AttendanceM* another);
	static void SendResult(AttendanceM * another);
	 
	 void RecvHuiZhiPro();

	 static void AttendPro(AttendanceM * another);//考勤终止条件判断
	bool AttendFiltration();//考勤过滤

signals:
	void chang();

public slots:
void Changeui();//

void attend();
void slotSign();
void attendList();//

	
private:
	void SetupUi();
public:
	QLabel * labelAttid;
	QLabel * labelAttname;
	QLabel * labelAttstarttime;
	QLabel * labelAttendtime;

	QLabel * edidAttid;
	QLabel * edidAttname;
	QLabel * edidAttstarttime;
	QLabel * edidAttendtime;

	QLabel * label1;
	TcpClient * tcpclient;
	QLineEdit * editinput;
	QPushButton * buttok;
	QPushButton * buttAttList;
	int m_method;
	CDeal deal;

	QString classroom;

	ATTEND * attendance;//当前考勤项目
	QVector<ATTEND> * already_attend;

	
};


#endif // ATTENDANCE_H
