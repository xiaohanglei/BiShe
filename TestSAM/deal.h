#pragma once
#include <QObject>
#include <windows.h>
#define ATTENDANCEMAX 100//���ο����������
#define MAX_BUFF 1024 * 1024//��֡��������С




class CDeal : public QObject
{
	Q_OBJECT

public:
	CDeal(QObject * parent = Q_NULLPTR);
	~CDeal();

	void FenLiZhen(UCHAR * recvbuff, int len);//��֡


private:
	UCHAR *m_All10ClientRecvBuf;//�������ݵĻ�����
	int m_buf10len;
};
