#include "tcpclient.hpp"

TcpClient::TcpClient(QString ip, int port, QObject * parent) : QObject(parent) 
{
	/*if ((clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		QMessageBox::information(0, tr("Error"), tr("Init NetWork Failed"), QMessageBox::Ok);
	}*/

	ServerPort = port;
	ServerIp = ip;

	sock = new QTcpSocket;

	//ConnectServer();

	//QObject::connect(sock, &QTcpSocket::readyRead, this, &TcpClient::RecvMsg);
		
}
TcpClient::TcpClient(QObject * parent) : QObject(parent)
{
	sock = new QTcpSocket;
}

void TcpClient::SetIp(QString ip)
{
	ServerIp = ip;
}
void TcpClient::SetPort(int port)
{
	ServerPort = port;
}


TcpClient::~TcpClient() 
{
	

}

void TcpClient::RecvMsg()
{
	QByteArray recvbyte;

	recvbyte = sock->readAll();
}

 
void TcpClient::LoadNet()
{

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		QMessageBox::information(0, tr("Error"),tr("Init NetWork Failed"), QMessageBox::Ok);
	}
}

bool TcpClient::ConnectServer()
{
	//取消已有的连接  
	sock->abort();

	sock->connectToHost(ServerIp, ServerPort);
	//等待连接成功  
	if (!sock->waitForConnected(30000))
	{
		
		return false;
	}
	
	return true;
	//设置服务器的网络地址结构

	/*sockaddr_in ServerAddress;

	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_port = htons(ServerPort);
	ServerAddress.sin_addr.S_un.S_addr = inet_addr(ServerIp.toLatin1().data());

	if ((connect(clientSocket, (sockaddr*)&ServerAddress, sizeof(ServerAddress))) == SOCKET_ERROR)
	{
		QMessageBox::information(0, tr("Error"), tr("Connect Server Failed"), QMessageBox::Ok);
		return;
	}*/
}

void TcpClient::SendMsg(QByteArray msg)
{
	sock->write(msg);
}
