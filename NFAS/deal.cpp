#include "deal.hpp"
#include <DataManager.h>
#include <QMessageBox>
Deal::Deal(QObject * parent) : QObject(parent) 
{
	m_All10ClientRecvBuf = new UCHAR[MAX_BUFF];
	m_buf10len = 0;
}

Deal::~Deal()
{
	
}

void Deal::FenLiZhen(UCHAR * recvbuff, int len, SOCKET sock, DataManager * dm)
{
#if 1
	if (len <= 0) return;
	//分离帧
	if ((m_buf10len + len) > MAX_BUFF)
	{
		m_buf10len = 0;
		return;
	}
	UCHAR * PTR = &m_All10ClientRecvBuf[0];
	memcpy(PTR + m_buf10len, (UCHAR*)recvbuff, len); //加入保存缓冲
	m_buf10len += len;

	if (m_buf10len <= 5) return;
	int j = 0;
	while (j < m_buf10len - 5)//循环次数 = 总长度-数据帧前5个字节（数据域长度4+标识1）
	{
		DWORD DATALEN = 0;

		memcpy(&DATALEN, PTR + j, 4);
		if (DATALEN > 5 * 1024 || DATALEN <= 0) //判断数据长度的有效性
		{
			j++;
			continue;
		}

		if ((j + 5 + DATALEN) > m_buf10len) //判断一帧的数据长度
		{
			j++;
			continue;
		}
		if (PTR[j + 5 + DATALEN - 1] != 0xFF) //判断数据包结尾标识
		{
			j++;
			continue;
		}
		memcpy(PTR, PTR + j, m_buf10len - j);
		m_buf10len -= j; //删除前面的数据

						 //剔除完整的数据
		memcpy((UCHAR*)recvbuff, PTR, DATALEN + 5); //临时保存数据内容，全部数据
		memcpy(PTR, PTR + DATALEN + 5, m_buf10len - DATALEN - 5);//完整帧后面的内容放到ptr中以便下次判断
		m_buf10len -= DATALEN;
		m_buf10len -= 5;
		if (m_buf10len < 0) m_buf10len = 0;

		if (recvbuff[4] == 0x0F)//判断心跳帧
		{
			j = 0;
			continue;;
		}
  
#endif		
		//处理数据

		//------------------------分离成功，开始解析数据帧------------------------------------
		NETBAO bao;
		memcpy(bao.mdstation, &recvbuff[5], 6);//目的站码
		bao.mdstation[6] = '\0';

		memcpy(bao.sostation, &recvbuff[11], 6);//源站码
		bao.sostation[6] = '\0';

		bao.baolx = recvbuff[17];
		if (bao.baolx != 0x01)//只处理命令
		{
			j = 0;
			continue;
		}

		bao.lxm = recvbuff[18];//类型码

		memcpy(bao.xinxibao, &recvbuff[19], DATALEN - 14);//详细信息包

		bao.sock = sock;		

		EnterCriticalSection(dm->GetCriNetBao());//命令入队，加锁

		
		dm->GetNetBao()->push_back(bao);//放到队列尾部
		
		LeaveCriticalSection(dm->GetCriNetBao());
		
		
		j = 0;
	}
}

void Deal::Deal_Attendance_MingLing(NETBAO bao)
{
}

void Deal::Deal_Attendance_HuiZhi(UCHAR * sostation, SOCKET sock, DataManager * dm)
{
	//找到该考勤设备在当前时段所负责的考勤项目

	
	QString tempQS;//考勤设备所在教室
	QStringList aclass;//保存需要考勤的考勤项目下辖的班级
	QString attendanceId;//需要考勤的考勤项目id
	ATTENDTIME atttime;//需要考勤的时段
	QVector<Student> students;//保存需要考勤的考勤项目下辖的所有学生列表

	bool flg = false;//是否找到需要考勤的考勤项目id


	//遍历所有考勤项目，找到需要考勤的考勤项目ID
	tempQS = QString(QLatin1String((char *)sostation));//查找考勤项目的考勤教室为该设备所在教室的考勤项目ID
	for (QVector<Attendance>::iterator it = dm->GetAttendance()->begin(); it != dm->GetAttendance()->end(); it++)
	{		
		if (it->GetMID() == tempQS )
		{
			//将在该教室考勤的项目的考勤时段进行遍历，找到符合该时段考勤的id
			
			QTime currdatetime = QTime::currentTime();//获得当前日期时间
	
			QString   weekday = QDateTime::currentDateTime().toString("ddd");//获得当前星期几

			QTime   currtimemin = currdatetime;//当前日期作为最小值
			QTime   currtimemax = currtimemin.addSecs(60 * MAX_ATTEND_TIME);//向后延迟 MAX_ATTEND_TIME 分钟

	
			
			//汉字的星期数换成字符数字
			if (QString::compare(weekday, tr("Monday")) == 0)
			{
				weekday = "1";
			}
			else if (QString::compare(weekday, tr("Tuesday")) == 0)
			{
				weekday = "2";
			}
			else if (QString::compare(weekday, tr("Wednesday")) == 0)
			{
				weekday = "3";
			}
			else if (QString::compare(weekday, tr("Thursday")) == 0)
			{
				weekday = "4";
			}
			else if (QString::compare(weekday, tr("Friday")) == 0)
			{
				weekday = "5";
			}
			
			//查找具体考勤时段
			for (QVector<ATTENDTIME> ::iterator ittime = it->attendancetime.begin(); ittime != it->attendancetime.end(); ittime++)
			{
				if (ittime->weekday == weekday)//如果有该星期的时段，比较具体的考勤时段的开始时间
				{
					//比较具体时段
					QTime	tempTime = QTime::fromString(ittime->starttime, "hh:mm");//
					if (tempTime > currtimemin && tempTime < currtimemax)   //判断当前考勤时段的开始时间是否在当前时间  至  其后的  MAX_ATTEND_TIME 分钟之内
					{
						attendanceId = it->GetID();
						aclass = it->GetAclass().split(",");//获得该考勤项目下辖的班级

						atttime = *ittime;//保存该项目的考勤时段

						flg = true;
						break;
					}

				}

			}
		}

		if (flg)
			break;
	}


	//根据考勤项目id找到找到该考勤项目下辖的所有学生信息
	
	for (int i = 0; i < aclass.size(); i++)
	{
		auto acla = aclass.at(i);
		for (auto iter = dm->GetStudent()->begin(); iter != dm->GetStudent()->end(); iter++)
		{
			if (iter->GetAclass() == acla)
			{
				students.append(*iter);
			}
		}
	}



	//组织数据，封装成数据帧

	//获取时间戳
	int istarttime = 0, iendtime = 0;

	QDateTime QDatetime1 = QDateTime::currentDateTime();
	QDatetime1.setTime(QTime::fromString(atttime.starttime,"HH:mm"));//将设置考勤开始时间
	
	istarttime = QDatetime1.toTime_t();//获得开始时间戳

	QDatetime1.setTime(QTime::fromString(atttime.endtime, "HH:mm"));//将设置考勤截至时间
	
	iendtime = QDatetime1.toTime_t();

	//

	int sendlen = 0;
	UCHAR * ptrSendData = new UCHAR[MAX_BUF_LEN];
	memset(ptrSendData, 0, MAX_BUF_LEN);
	ptrSendData[4] = 0x8f;

	memcpy(&ptrSendData[5], sostation, 6);//将考勤设备的源电报码作为回执的目的电报码

	UCHAR mdstation[7] = "Server";    
	memcpy(&ptrSendData[11],mdstation, 6);//填写服务器为回执的源电报码

	sendlen += 12;

	ptrSendData[17] = 0x00;//数据
	ptrSendData[18] = 0x11;//数据

	sendlen += 2;

	//考勤项目编号
	QByteArray tempQB = attendanceId.toLatin1();
	char * id = new char[tempQB.size()];
	id = tempQB.data();
	memcpy(&ptrSendData[19], id, 8);//

	sendlen += 8;

	memcpy(&ptrSendData[27], &istarttime, 4);//开始时间
	memcpy(&ptrSendData[31], &iendtime, 4);//结束时间

	sendlen += 8;
	
	WORD count = students.size();//需要考勤的学生数量

	memcpy(&ptrSendData[35], &count, 2);//记录数

	sendlen += 2;



	QByteArray tempsid;
	QByteArray tempsfig;
	char * sid = new char[8];
	char * sfig = new char[32];

	for (int i = 0; i < count; i++)
	{
		tempsid = students.at(i).GetID().toLatin1();
		tempsfig = students.at(i).GetFigure().toLatin1();
		sid = tempsid.data();
		sfig = tempsfig.data();

		memcpy(&ptrSendData[37 + i * 40], sid, 8);//学号
		memcpy(&ptrSendData[45 + i * 40] , sfig, 32);//指纹
	}
	sendlen += count * 40;

	ptrSendData[sendlen + 5] = 0xff;//结尾标记

	sendlen += 1;

	memcpy(&ptrSendData[0], &sendlen, 4);//填充data_len的大小
	

	//发送到该设备

	int sendvlen = 0;
	if ((sendvlen = send(sock, (char*)ptrSendData, sendlen + 5, 0)) == SOCKET_ERROR)
	{
		QMessageBox::information(0, tr("Feedback failure"), tr("fan kui shi bai ,kao qin jiao shi wei :"), QMessageBox::Ok);
		return;
	}

}

void Deal::Deal_Result_MingLing(NETBAO bao)
{
}

void Deal::Deal_Result_HuiZhi(UCHAR *sostation, SOCKET sock)
{
}
