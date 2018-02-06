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
		if (recvbuff[18] == 0x11)//�жϻ�ִ����
		{
			attendance->Stuends.clear();
			//����������
			if (recvbuff[19] == 0x01)//�Ƿ��п�������
			{
				//������������������.
				
				char attendid[9] = { 0 };
				int istarttime = 0;
				int iendtime = 0;
				WORD count = 0;

				char ** stuid = nullptr;
				char ** stufing = nullptr;

				memcpy(attendid, &recvbuff[20], 8);//���ڱ��
				memcpy(&istarttime, &recvbuff[28], 4);//��ʼʱ��
				memcpy(&iendtime, &recvbuff[32], 4);//����ʱ��

				attendance->attendanceid = QString(attendid);
				attendance->starttime = istarttime;
				attendance->endtiem = iendtime;
				
				attendance->signcount = 0;

				memcpy(&count, &recvbuff[36], 2);//��¼����
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
			emit RecvAttenance();//�������������������Ժ󣬷���һ���źţ�
			
		}
		else//���ڼ�¼������
		{
			//��ʱ���Կ��ڼ�¼�������Ļ�ִ
			if (recvbuff[19] == 0x01)//����ɹ�
			{

			}
		}
		
	
		
	}
}
