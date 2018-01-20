#pragma once

#include <windows.h>
#include <process.h>
#include <vector>
#include <QString>
#include <QVector>



struct CLIENTLIST//在线
{
	int num;//序号
	char name[50];//客户端名称
	char ip[20];//ip地址
	
	SOCKET sock;//套接字
};

struct ALLCLIENTLIST//全部
{
	char name[50];//客户端名称
	char ip[20];//ip地址
};

class TcpServer
{
public:
	char m_IpAadress[32];//ip地址
	u_short m_Port;//端口
	SOCKET m_ServerSock;//服务端sock

	std::vector<CLIENTLIST> m_ClientList;//在线客户端列表
	//QVector <ALLCLIENTLIST> m_AllClientList;//全部客户端列表
	

	CRITICAL_SECTION g_cs;//临界区变量

public:
	TcpServer();
	~TcpServer();

	BOOL LoadWinSock();//加载winsock库
	BOOL StartServer();//启动服务器

	SOCKET * GetServerSock();


	static void RecvClientProc(LPVOID another);//接收客户端连接的线程
	static void RecvDataProc(LPVOID another);//接收数据的线程

private:

};

typedef struct
{
	TcpServer *tcp;
	SOCKET ssock;
}THREADARG;

