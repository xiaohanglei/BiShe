#include "TcpServer.h"

#include <QMessageBox>
#include <QSettings>
#include <QFile>
#include <DataManager.h>
#pragma comment(lib,"ws2_32.lib")

TcpServer::TcpServer()
{
	InitializeCriticalSection(&g_cs);//初始化线程锁
}


TcpServer::~TcpServer()
{
	closesocket(m_ServerSock);

	for (std::vector<CLIENTLIST>::iterator it = m_ClientList.begin();
		it != m_ClientList.end(); it++)
	{
		//依次关闭交每一个客户端的sock
		closesocket((*it).sock);
	}
	WSACleanup();

	DeleteCriticalSection(&g_cs);
}

BOOL TcpServer::LoadWinSock()
{
	//初始化版本号
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		QMessageBox::information(0, "出错","初始化网络环境失败！", QMessageBox::Ok);
		return FALSE;
	}
	return TRUE;
}

BOOL TcpServer::StartServer()
{
	
	//创建套接字
	if ((this->m_ServerSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		QMessageBox::information(0, "出错", "创建套接字失败，服务器启动失败！", QMessageBox::Ok);
		return FALSE;
	}

	//建立服务器端的网络地址结构
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
		QMessageBox::information(0, "出错", "地址绑定失败！", QMessageBox::Ok);//,WSAGetLastError()
		return FALSE;
	}

	//启动服务器成功以后，开始监听对服务器的访问

	if ((listen(m_ServerSock, SOMAXCONN)) == SOCKET_ERROR)
	{
		QMessageBox::information(0, "出错", "监听失败！", QMessageBox::Ok);
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
	TcpServer *tcp = (TcpServer *)another;

	while (1)
	{
		Sleep(5000);//间隔时间 5s

		//创建客户端的套接字连接
		SOCKET * SocketClient = new SOCKET;
		sockaddr_in ClientAddress;
		memset(&ClientAddress, 0, sizeof(ClientAddress));
		int lenCA = sizeof(ClientAddress);

		//连接客户端的套接字

		if ((*SocketClient = accept(*(tcp->GetServerSock()), (sockaddr*)&ClientAddress, &lenCA)) == INVALID_SOCKET)
		{
			//AfxMessageBox("客户端连接失败!");
			return;
		}
		else//连接成功
		{
			//连接成功以后，将客户端的信息加入到客户端列表里边
			
			CLIENTLIST tempClient;//构造客户端列表对象
			tempClient.sock = *SocketClient;
			strcpy(tempClient.name, "未知客户端");

			//获取IP,并查找配置文件中是否有该客户端的信息
			sprintf(tempClient.ip, inet_ntoa(ClientAddress.sin_addr));
			for (QVector<ALLCLIENTLIST>::iterator it = tcp->m_AllClientList.begin(); it != tcp->m_AllClientList.end(); it++)//遍历所有客户端列表
			{
				if (strcmp(it->ip, tempClient.ip) == 0)
				{
					strcpy(tempClient.name, it->name);
					break;
				}
			}
			tcp->m_ClientList.push_back(tempClient);//放入在线客户端列表
			THREADARG * tmp = new THREADARG;
			tmp->tcp = tcp;
			tmp->ssock = *SocketClient;

			_beginthread(TcpServer::RecvDataProc, 0,tmp);//为该客户端开辟一个接收数据的线程

		}

	}

}

void TcpServer::RecvDataProc(LPVOID another)
{
	THREADARG * tmp = (THREADARG *)another;
	UCHAR *Recvbuf = new UCHAR[MAX_BUF_LEN];
	int Bytes = 0;//接受的长度

	while (1)
	{
		if ((Bytes = recv(tmp->ssock, (char *)Recvbuf, MAX_BUF_LEN, 0)) <= 0/* == SOCKET_ERROR*/)
		{
			//AfxMessageBox("断开连接!");
			//说明该客户端已经断开连接,此时应该将该客户端的sock从客户端列表中移除
			delete Recvbuf;
			return;		//然后结束该线程
		}
		Recvbuf[Bytes] = '\0';//给接收的数据末尾加 结束标记	

		//----------------------------------------------------------接收成功以后，调用处理模块进行数据处理帧
		
	}

	delete Recvbuf;
}
