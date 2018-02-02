#ifndef TESTSAM_H
#define TESTSAM_H

#include <QtWidgets/QWidget>
//#include "ui_testsam.h"
#include <QLabel>

#include <QPushButton>

#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QButtonGroup>
#include <QMessageBox>
#include <QRadioButton>
#include "tcpclient.hpp"

class TestSAM : public QWidget
{
	Q_OBJECT

public:
	TestSAM(QWidget *parent = 0);
	~TestSAM();

public slots:

void slotSubmit();
void slotTestSend();
void RecvMsg();


private:
	void SetupUi();

	QPushButton * buttTestSendMsg;
	TcpClient *tcpclient;

	QHBoxLayout * main_layout;

	QLineEdit * editServerIp;
	QLineEdit * editServerPort;
	QLineEdit * editAttendClassRoom;

	QButtonGroup * groupAttend;
	QRadioButton * radioTable;
	QRadioButton * radioStuId;

	QPushButton * submit;




};

#endif // TESTSAM_H
