#include "deal.h"
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

void Deal::FenLiZhen(UCHAR * recvbuff, int len)
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
		//判断回执数据类型
		if (recvbuff[17] == 0x11)//待考勤数据
		{
			if (recvbuff[19] == 0x01)//有考勤数据
			{

			}
		}
		else if (recvbuff[17] == 0x12)//考勤记录处理结果
		{
			if (recvbuff[19] == 0x01)//处理成功 
			{

			}
		}
	
	}
}
