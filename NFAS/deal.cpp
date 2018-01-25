#include "deal.hpp"
#include <DataManager.h>
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

	QStringList attendanceIdList;
	QString tempQS;

	for (QVector<Attendance>::iterator it = dm->GetAttendance()->begin(); it != dm->GetAttendance()->end(); it++)
	{
		tempQS = QString(QLatin1String((char *)sostation));//查找考勤项目的考勤教室为该设备所在教室的考勤项目ID
		if (it->GetMID() == tempQS )
		{
			attendanceIdList << it->GetID();//将该考勤项目的ID放入列表中
		}
	}


	//组织数据，封装成数据帧



	//发送到该设备

}

void Deal::Deal_Result_MingLing(NETBAO bao)
{
}

void Deal::Deal_Result_HuiZhi(UCHAR *sostation, SOCKET sock)
{
}
