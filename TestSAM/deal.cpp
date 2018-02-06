#include "deal.h"
//#include <DataManager.h>
#include <QMessageBox>
CDeal::CDeal(QObject * parent) : QObject(parent)
{
	m_All10ClientRecvBuf = new UCHAR[MAX_BUFF];
	m_buf10len = 0;

}

CDeal::~CDeal()
{

}

void CDeal::FenLiZhen(UCHAR * recvbuff, int len, ATTEND * attendance)
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
		if (recvbuff[18] == 0x11)//判断回执类型
		{
			attendance->Stuends.clear();
			//待考勤名单
			if (recvbuff[19] == 0x01)//是否有考勤数据
			{
				//将考勤名单解析出来.
				
				char attendid[9] = { 0 };
				int istarttime = 0;
				int iendtime = 0;
				WORD count = 0;

				char ** stuid = nullptr;
				char ** stufing = nullptr;

				memcpy(attendid, &recvbuff[20], 8);//考勤编号
				memcpy(&istarttime, &recvbuff[28], 4);//开始时间
				memcpy(&iendtime, &recvbuff[32], 4);//结束时间

				attendance->attendanceid = QString(attendid);
				attendance->starttime = istarttime;
				attendance->endtiem = iendtime;
				
				attendance->signcount = 0;

				memcpy(&count, &recvbuff[36], 2);//记录数量
				stuid = new char*[count];
				stufing = new char*[count];

				STUDENT tempStudent;
				for (int i = 0; i < count; i++)
				{
					stuid[i] = new char[9];
					stufing[i] = new char[33];
					memset(stuid[i], 0, 9);
					memset(stufing[i], 0, 33);

					memcpy(stuid[i], &recvbuff[38 + i * 40], 8);
					memcpy(stufing[i], &recvbuff[46 + i * 40], 32);

					tempStudent.StuId = QString(stuid[i]);
					tempStudent.StuFinger = QString(stufing[i]);
					tempStudent.StuSign = false;
					attendance->Stuends.append(tempStudent);

				}
				
			}
			emit RecvAttenance();//将考勤名单解析出来以后，发射一个信号，
			
		}
		else//考勤记录处理结果
		{
			//暂时忽略考勤记录处理结果的回执
			if (recvbuff[19] == 0x01)//处理成功
			{

			}
		}
		
	
		
	}
}
