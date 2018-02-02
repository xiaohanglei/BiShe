#ifndef NFAS_H
#define NFAS_H

#include <QDialog>
#include "DataManager.h"
#include "deal.hpp"
#include <QCloseEvent>

class QTabWidget;


//������
class NFAS : public QDialog
{
	Q_OBJECT

public:
	NFAS(DataManager *dm, QWidget *parent = 0);
	~NFAS();


	static void ExeMingLingProc(LPVOID another);//�������豸�������߳� 
	void closeEvent(QCloseEvent *event);    // ��дcloseEvent������

public slots:
	void UpdateTab(int index);


private:
	void setupUi();
	QTabWidget* main_tab;//ѡ��ؼ�
	DataManager* dataManager;

	bool IsNetWork;//�жϱ��ͻ����Ƿ�������ͨѶ����

	
};

#endif // NFAS_H
