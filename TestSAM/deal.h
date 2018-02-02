#pragma once
#include <QObject>
#include <windows.h>
#define ATTENDANCEMAX 100//单次考勤最大数量
#define MAX_BUFF 1024 * 1024//分帧缓冲区大小




class Deal : public QObject
{
	Q_OBJECT

public:
	Deal(QObject * parent = Q_NULLPTR);
	~Deal();

	void FenLiZhen(UCHAR * recvbuff, int len);//分帧


private:
	UCHAR *m_All10ClientRecvBuf;//解析数据的缓冲区
	int m_buf10len;
};
