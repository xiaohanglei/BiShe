#pragma once
#include <QObject>
#include <windows.h>
#include <QVector>
#define ATTENDANCEMAX 100//���ο����������
#define MAX_BUFF 1024 * 1024//��֡��������С
#include <QTextCodec>  

struct STUDENT
{
	QString StuId;
	QString StuFinger;
	bool StuSign;
};

struct ATTEND
{
	QString attendanceid;
	QString attendancename;
	int starttime;
	int endtiem;
	QVector<STUDENT> Stuends;

	int signcount;//�Ѿ�ǩ������
};



class CDeal : public QObject
{
	Q_OBJECT

public:
	CDeal(QObject * parent = Q_NULLPTR);
	~CDeal();

	void FenLiZhen(UCHAR * recvbuff, int len, ATTEND * attend);//��֡

signals:
	void RecvAttenance();


private:
	UCHAR *m_All10ClientRecvBuf;//�������ݵĻ�����
	int m_buf10len;
	QTextCodec *codec = QTextCodec::codecForName("GBK");//ָ��QString�ı��뷽ʽ
};
