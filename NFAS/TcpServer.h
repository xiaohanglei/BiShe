#pragma once

#include <windows.h>
#include <process.h>
#include <vector>
#include <QString>
#include <QVector>



struct CLIENTLIST//����
{
	int num;//���
	char name[50];//�ͻ�������
	char ip[20];//ip��ַ
	
	SOCKET sock;//�׽���
};

struct ALLCLIENTLIST//ȫ��
{
	char name[50];//�ͻ�������
	char ip[20];//ip��ַ
};

class TcpServer
{
public:
	char m_IpAadress[32];//ip��ַ
	u_short m_Port;//�˿�
	SOCKET m_ServerSock;//�����sock

	std::vector<CLIENTLIST> m_ClientList;//���߿ͻ����б�
	//QVector <ALLCLIENTLIST> m_AllClientList;//ȫ���ͻ����б�
	

	CRITICAL_SECTION g_cs;//�ٽ�������

public:
	TcpServer();
	~TcpServer();

	BOOL LoadWinSock();//����winsock��
	BOOL StartServer();//����������

	SOCKET * GetServerSock();


	static void RecvClientProc(LPVOID another);//���տͻ������ӵ��߳�
	static void RecvDataProc(LPVOID another);//�������ݵ��߳�

private:

};

typedef struct
{
	TcpServer *tcp;
	SOCKET ssock;
}THREADARG;

