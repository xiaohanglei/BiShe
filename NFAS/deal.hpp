#pragma once
#include <QObject>
#include <windows.h>
#include "DataManager.h"
#define ATTENDANCEMAX 100//单次考勤最大数量
#define MAX_BUFF 1024 * 1024//分帧缓冲区大小




class Deal : public QObject 
{
	Q_OBJECT

public:
	Deal(QObject * parent = Q_NULLPTR);
	~Deal();

	void FenLiZhen(UCHAR * recvbuff, int len, SOCKET sock, DataManager * dm);//分帧

	

	static void Deal_Attendance_MingLing(NETBAO bao);//请求考勤数据命令
	static void Deal_Attendance_HuiZhi(UCHAR *sostation, SOCKET sock, DataManager * dm);//回执待考勤数据
	
	static void Deal_Result_MingLing(NETBAO bao);//处理考勤设备推送的考勤结果
	static void Deal_Result_HuiZhi(UCHAR *sostation, SOCKET sock);//回执给考勤设备的处理结果


private:
	UCHAR *m_All10ClientRecvBuf;//解析数据的缓冲区
	int m_buf10len;
	
};
