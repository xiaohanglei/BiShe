#pragma once
#include <QObject>
#include <windows.h>
#include <QVector>
#define ATTENDANCEMAX 100//单次考勤最大数量
#define MAX_BUFF 1024 * 1024//分帧缓冲区大小
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

	int signcount;//已经签到人数
};



class CDeal : public QObject
{
	Q_OBJECT

public:
	CDeal(QObject * parent = Q_NULLPTR);
	~CDeal();

	void FenLiZhen(UCHAR * recvbuff, int len, ATTEND * attend);//分帧

signals:
	void RecvAttenance();


private:
	UCHAR *m_All10ClientRecvBuf;//解析数据的缓冲区
	int m_buf10len;
	QTextCodec *codec = QTextCodec::codecForName("GBK");//指定QString的编码方式
};
