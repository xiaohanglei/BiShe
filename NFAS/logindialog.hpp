#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H
#include <QDialog>
#include "DataManager.h"
#include <QPushButton>
#include <QLineEdit>

class LoginDialog : public QDialog {
	Q_OBJECT

public:
	LoginDialog(DataManager* dm, QWidget * parent = 0);
	~LoginDialog();

public slots:
	void Login();

private:
	void setupUi();
	DataManager* dataManager;
	QLineEdit* userid;
	QLineEdit* userpassword;
	QPushButton* login;
};
#endif // LOGINDIALOG_H