#pragma once
#include <QObject>
#include <WinSock2.h>
#include <Windows.h>
#include <QMessageBox>
#include <QtNetwork/QtNetwork>

#pragma comment (lib,"WS2_32.lib")

class TcpClient : public QObject 
{
	Q_OBJECT

public:
	TcpClient(QString ip, int port ,QObject * parent = Q_NULLPTR);
	TcpClient(QObject * parent = Q_NULLPTR);
	~TcpClient();
	void SetIp(QString ip);
	void SetPort(int port);
	QTcpSocket * GetSock()
	{
		return sock;
	}
	void LoadNet();//初始化网络环境
	bool ConnectServer();
	void SendMsg(QByteArray msg);
public slots:
	void RecvMsg();

	

private:

	SOCKET clientSocket;
	QTcpSocket *sock;
	QString ServerIp;
	int ServerPort;
	
};
