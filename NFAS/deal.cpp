#include "deal.hpp"
//#include <DataManager.h>
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

	//组织数据，封装成数据帧
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

	if (students.size() != 0)//判断有无考勤数据
	{
		ptrSendData[19] = 0x01;//表示有考勤数据
		sendlen += 1;
		//获取时间戳
		int istarttime = 0, iendtime = 0;

		QDateTime QDatetime1 = QDateTime::currentDateTime();
		QDatetime1.setTime(QTime::fromString(atttime.starttime, "HH:mm"));//将设置考勤开始时间

		istarttime = QDatetime1.toTime_t();//获得开始时间戳

		QDatetime1.setTime(QTime::fromString(atttime.endtime, "HH:mm"));//将设置考勤截至时间

		iendtime = QDatetime1.toTime_t();

		//考勤项目编号
		QByteArray tempQB = attendanceId.toLatin1();
		char *id = nullptr;
		id = tempQB.data();
		memcpy(&ptrSendData[20], id, 8);//

		sendlen += 8;

		memcpy(&ptrSendData[28], &istarttime, 4);//开始时间
		memcpy(&ptrSendData[32], &iendtime, 4);//结束时间

		sendlen += 8;

		WORD count = students.size();//需要考勤的学生数量

		memcpy(&ptrSendData[36], &count, 2);//记录数

		sendlen += 2;

		QByteArray tempsid;
		QByteArray tempsfig;
		char * sid =nullptr;
		char * sfig = nullptr;

		for (int i = 0; i < count; i++)
		{
			tempsid = students.at(i).GetID().toLatin1();
			tempsfig = students.at(i).GetFigure().toLatin1();
			sid = tempsid.data();
			sfig = tempsfig.data();

			memcpy(&ptrSendData[38 + i * 40], sid, 8);//学号
			memcpy(&ptrSendData[46 + i * 40], sfig, 32);//指纹
		}
		sendlen += count * 40;	
	}
	else
	{
		ptrSendData[19] = 0x00;//表示没有考勤数据
		sendlen += 1;
	}
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

	delete ptrSendData;
}

void Deal::Deal_Result_MingLing(NETBAO bao, DataManager * dm)
{
	//解析命令
	UCHAR * ptrXinXiBao = bao.xinxibao;
	char attendid[9] = { 0 };//考勤项目ID
	
	WORD count = 0;//记录数
				   //QVector<STATTEND> students;

	//RESULT resultnet;
	RESULT *resultnet = new RESULT;

	memcpy(attendid, &ptrXinXiBao[0], 8);
	
	memcpy(&count, &ptrXinXiBao[8], 2);

	resultnet->attendid = QString(QLatin1String(attendid));

	QDateTime currdatetime = QDateTime::currentDateTime();

	resultnet->resultid = resultnet->attendid + "-" + currdatetime.toString("yyyyMMddHHmmss");

	char sid[9] = { 0 };//
	bool isq;
	STATTEND tempstud;
	for (int i = 0; i < count; i++)
	{
		memcpy(sid, &ptrXinXiBao[10 + i * 9], 8);

		tempstud.sid = QString(QLatin1String(sid));
		if (ptrXinXiBao[18 + i * 9] == 0x01)
			tempstud.isq = true;
		else
			tempstud.isq = false;

		resultnet->students.append(tempstud);
	}

	bool ret = Result_Db(resultnet, dm);//考勤结果直接入库，不用再界面中显示
	

	//回执

	Deal_Result_HuiZhi(bao.sostation, bao.sock , ret);

}

bool Deal::Result_Db(RESULT * result, DataManager * dataManager)
{
	Result attendanceresult;

	int absence = 0;
	int leave = 0;
	QStringList absencsstudent;//缺勤学生的列表
	QStringList leavestudent;//请假学生的列表 
	int total = result->students.size();//考勤结果中学生的个数
	for (int i = 0; i < total; i++)
	{
		QString itemid = result->students.at(i).sid;
		//QString itemname;
		//QString itemaca;
		//QString itemcla;
		//QString itemfigure ;
		bool iteminclass = result->students.at(i).isq;//到课
		//QString itemleaveclass ;//请假
		QString itemabclass ;//缺勤

		if (!iteminclass) //只要没签到
		{
			//if (itemleaveclass == 1) //如果没有签到，则判断是否请假
			//{
			//	leave++;//请假次数+1
			//	leavestudent << itemid;//将该学生的id存入请假列表中
			//}
		
			//if (itemabclass == 1)
			//{
			absence++;//缺勤人数+1
			absencsstudent << itemid;//将缺勤的学生id放入列表中
			//}

		}
	}
	QString attenfileid = result->attendid;//取出考勤项目编号
	attendanceresult.SetAID(attenfileid);

	attendanceresult.SetRID(result->resultid);//考勤结果id
	attendanceresult.SetANum(total - absence - leave);//实到人数
	attendanceresult.SetTNum(total);//应签到人数
	attendanceresult.SetLStu(leavestudent.join(","));//将学生id列表
	attendanceresult.SetAStu(absencsstudent.join(","));
	
	attendanceresult.SetUID("No Submit");
	//找到该考勤项目的负责人，作为提交人
	for (auto it = dataManager->GetAttendance()->begin(); it != dataManager->GetAttendance()->end(); it++)
	{
		if (it->GetID() == attenfileid)
		{
			attendanceresult.SetUID(it->GetLeader());
		}
	}


	if (dataManager->ResultOP(attendanceresult, 0)) //将该考勤结果插入数据库
	{
		dataManager->updateResult(dataManager->GetCurrentUser());//从数据库中重新加载考勤结果数据

		for (auto it = dataManager->GetResult()->begin(); it != dataManager->GetResult()->end(); it++)
		{
			auto abstu = it->GetAStu().split(",");//缺勤学生的id列表
			auto lestu = it->GetLStu().split(",");//请假学生的id列表

			if (it->GetAID() == attendanceresult.GetAID() && it->GetRID() == attendanceresult.GetRID())//是否是当前的考勤结果
			{
				for (auto iter = dataManager->GetAttendance()->begin(); iter != dataManager->GetAttendance()->end(); iter++)//是否是当前的考勤项目
				{
					if (iter->GetID() == it->GetAID())//考勤结果的考勤编号和考勤项目的编号是否匹配
					{
						auto aclass = iter->GetAclass().split(",");//该考勤项目所属的考勤班级列表
						for (int i = 0; i < aclass.size(); i++)//遍历该班级列表
						{
							for (auto its = dataManager->GetStudent()->begin(); its != dataManager->GetStudent()->end(); its++)
							{
								//遍历所有学生列表，找到该班级的所有学生
								if (its->GetAclass() == aclass[i])//判断该学生所在班级和当前班级是否匹配
								{
									its->AddTotime();//考勤次数+1

													 //统计该学生的考勤情况更新到数据库中
									if (abstu.indexOf(its->GetID()) >= 0)//通过索引号判断该学生是否在缺勤列表中
									{
										its->AddAbtime();//缺勤次数+1

														 //更新该学生在数据库中信息
										dataManager->StudentOP(Student(its->GetID(), "", 0, "", "", "", 0, 0, its->GetAbtimes(), its->GetTotimes()), 5);

									}
									else if (lestu.indexOf(its->GetID()) >= 0)
									{
										its->AddLetime();
										dataManager->StudentOP(Student(its->GetID(), "", 0, "", "", "", 0, its->GetLetimes(), 0, its->GetTotimes()), 4);
									}
									else
									{
										its->AddIntime();
										dataManager->StudentOP(Student(its->GetID(), "", 0, "", "", "", its->GetIntimes(), 0, 0, its->GetTotimes()), 3);
									}
								}
							}
						}
						break;
					}
				}
				break;
			}
		}
		//updateTree();
	}
	else
	{
		return false;
	}

	return true;
}

void Deal::Deal_Result_HuiZhi(UCHAR *sostation, SOCKET sock,bool ret)
{
	//组织数据，封装成数据帧
	//

	int sendlen = 0;
	UCHAR * ptrSendData = new UCHAR[MAX_BUF_LEN];
	memset(ptrSendData, 0, MAX_BUF_LEN);
	ptrSendData[4] = 0x8f;

	memcpy(&ptrSendData[5], sostation, 6);//将考勤设备的源电报码作为回执的目的电报码

	UCHAR mdstation[7] = "Server";
	memcpy(&ptrSendData[11], mdstation, 6);//填写服务器为回执的源电报码

	sendlen += 12;

	ptrSendData[17] = 0x00;//数据
	ptrSendData[18] = 0x12;//处理结果

	sendlen += 2;
	if (ret)//根据处理结果返回不同的值
		ptrSendData[19] = 0x01;//00 表示出错，01表示成功
	else
		ptrSendData[19] = 0x00;//00 表示出错，01表示成功

	sendlen += 1;

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

	delete ptrSendData;


}

