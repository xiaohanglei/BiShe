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
	//����֡
	if ((m_buf10len + len) > MAX_BUFF)
	{
		m_buf10len = 0;
		return;
	}
	UCHAR * PTR = &m_All10ClientRecvBuf[0];
	memcpy(PTR + m_buf10len, (UCHAR*)recvbuff, len); //���뱣�滺��
	m_buf10len += len;

	if (m_buf10len <= 5) return;
	int j = 0;
	while (j < m_buf10len - 5)//ѭ������ = �ܳ���-����֡ǰ5���ֽڣ������򳤶�4+��ʶ1��
	{
		DWORD DATALEN = 0;

		memcpy(&DATALEN, PTR + j, 4);
		if (DATALEN > 5 * 1024 || DATALEN <= 0) //�ж����ݳ��ȵ���Ч��
		{
			j++;
			continue;
		}

		if ((j + 5 + DATALEN) > m_buf10len) //�ж�һ֡�����ݳ���
		{
			j++;
			continue;
		}
		if (PTR[j + 5 + DATALEN - 1] != 0xFF) //�ж����ݰ���β��ʶ
		{
			j++;
			continue;
		}
		memcpy(PTR, PTR + j, m_buf10len - j);
		m_buf10len -= j; //ɾ��ǰ�������

						 //�޳�����������
		memcpy((UCHAR*)recvbuff, PTR, DATALEN + 5); //��ʱ�����������ݣ�ȫ������
		memcpy(PTR, PTR + DATALEN + 5, m_buf10len - DATALEN - 5);//����֡��������ݷŵ�ptr���Ա��´��ж�
		m_buf10len -= DATALEN;
		m_buf10len -= 5;
		if (m_buf10len < 0) m_buf10len = 0;

		if (recvbuff[4] == 0x0F)//�ж�����֡
		{
			j = 0;
			continue;;
		}

#endif		
		//��������

		//------------------------����ɹ�����ʼ��������֡------------------------------------
		//�жϻ�ִ��������
		if (recvbuff[17] == 0x11)//����������
		{
			if (recvbuff[19] == 0x01)//�п�������
			{

			}
		}
		else if (recvbuff[17] == 0x12)//���ڼ�¼������
		{
			if (recvbuff[19] == 0x01)//����ɹ� 
			{

			}
		}
	
	}
}
