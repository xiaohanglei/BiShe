#include "TcpServer.h"

#include <QMessageBox>
#include <QSettings>
#include <QFile>
#include <DataManager.h>
#pragma comment(lib,"ws2_32.lib")

#include "Deal.hpp"

TcpServer::TcpServer()
{
	InitializeCriticalSection(&g_cs);//��ʼ���߳���
}


TcpServer::~TcpServer()
{
	closesocket(m_ServerSock);

	for (std::vector<CLIENTLIST>::iterator it = m_ClientList.begin();
		it != m_ClientList.end(); it++)
	{
		//���ιرս�ÿһ���ͻ��˵�sock
		closesocket((*it).sock);
	}
	WSACleanup();

	DeleteCriticalSection(&g_cs);
}

BOOL TcpServer::LoadWinSock()
{
	//��ʼ���汾��
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		QMessageBox::information(0, "����","��ʼ�����绷��ʧ�ܣ�", QMessageBox::Ok);
		return FALSE;
	}
	return TRUE;
}

BOOL TcpServer::StartServer()
{
	
	//�����׽���
	if ((this->m_ServerSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		QMessageBox::information(0, QStringLiteral("����"), QStringLiteral("Failure to create a socket, server startup failure"), QMessageBox::Ok);
		return FALSE;
	}

	//�����������˵������ַ�ṹ
	struct sockaddr_in ServerAddress;
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_port = htons(m_Port);



	if (strcmp("0", m_IpAadress) == 0)
		ServerAddress.sin_addr.S_un.S_addr = INADDR_ANY;
	else
		ServerAddress.sin_addr.S_un.S_addr = inet_addr(m_IpAadress);

	int ret;
	ret = bind(m_ServerSock, (sockaddr*)&ServerAddress, sizeof(ServerAddress));
	if (ret == SOCKET_ERROR)
	{
		QMessageBox::information(0, QStringLiteral("����"), QStringLiteral("��ַ��ʧ�ܣ�"), QMessageBox::Ok);//,WSAGetLastError()
		return FALSE;
	}

	//�����������ɹ��Ժ󣬿�ʼ�����Է������ķ���

	if ((listen(m_ServerSock, SOMAXCONN)) == SOCKET_ERROR)
	{
		QMessageBox::information(0, QStringLiteral("����"), QStringLiteral("����ʧ�ܣ�"), QMessageBox::Ok);
		return FALSE;
	}
	return TRUE;
}

SOCKET * TcpServer::GetServerSock()
{
	return &m_ServerSock;
}

void TcpServer::RecvClientProc(LPVOID another)
{
	DataManager * datamanager = (DataManager*)another;
	TcpServer *tcp = datamanager->GetTcp();

	while (1)
	{
		Sleep(5000);//���ʱ�� 5s

		//�����ͻ��˵��׽�������
		SOCKET * SocketClient = new SOCKET;
		sockaddr_in ClientAddress;
		memset(&ClientAddress, 0, sizeof(ClientAddress));
		int lenCA = sizeof(ClientAddress);

		//���ӿͻ��˵��׽���

		if ((*SocketClient = accept(*(tcp->GetServerSock()), (sockaddr*)&ClientAddress, &lenCA)) == INVALID_SOCKET)
		{
			//AfxMessageBox("�ͻ�������ʧ��!");
			return;
		}
		else//���ӳɹ�
		{
			//���ӳɹ��Ժ󣬽��ͻ��˵���Ϣ���뵽�ͻ����б����
			
			CLIENTLIST tempClient;//����ͻ����б����
			tempClient.sock = *SocketClient;
			strcpy(tempClient.name, "δ֪�ͻ���");
			sprintf(tempClient.ip, inet_ntoa(ClientAddress.sin_addr));

			EnterCriticalSection(&tcp->g_cs);//�ͻ����б���������߳���

			//��ȡIP,�����������ļ����Ƿ��иÿͻ��˵���Ϣ
			
			for (QVector<Device>::iterator it = datamanager->GetDevice()->begin(); it != datamanager->GetDevice()->end(); it++)//���������豸�б�
			{
				if (strcmp(it->GetIp().toLatin1(), tempClient.ip) == 0)
				{
					strcpy(tempClient.name, it->GetName().toLatin1());
					break;
				}
			}
			tcp->m_ClientList.push_back(tempClient);//�������߿ͻ����б�

			LeaveCriticalSection(&tcp->g_cs);//�⿪�߳���


			THREADARG * tmp = new THREADARG;
			tmp->dm = datamanager;
			tmp->ssock = *SocketClient;
			_beginthread(TcpServer::RecvDataProc, 0,tmp);//Ϊ�ÿͻ��˿���һ���������ݵ��߳�
		}
	}

}

void TcpServer::RecvDataProc(LPVOID another)
{
	THREADARG * tmp = (THREADARG *)another;
	UCHAR *Recvbuf = new UCHAR[MAX_BUF_LEN];
	int Bytes = 0;//���ܵĳ���

	Deal de;

	while (1)
	{
		if ((Bytes = recv(tmp->ssock, (char *)Recvbuf, MAX_BUF_LEN, 0)) <= 0/* == SOCKET_ERROR*/)
		{
			//AfxMessageBox("�Ͽ�����!");
			//˵���ÿͻ����Ѿ��Ͽ�����,��ʱӦ�ý��ÿͻ��˵�sock�ӿͻ����б����Ƴ�
			delete Recvbuf;
			return;		//Ȼ��������߳�
		}
		Recvbuf[Bytes] = '\0';//�����յ�����ĩβ�� �������	

		//----------------------------------------------------------���ճɹ��Ժ󣬵��ô���ģ��������ݴ���֡	
		de.FenLiZhen(Recvbuf, Bytes,tmp->ssock,tmp->dm);

	}

	delete Recvbuf;
}
