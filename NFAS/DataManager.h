#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QObject>
#include <QVector>
#include <QList>

#include "TcpServer.h"



#define  MAX_BUF_LEN 1024*1024
struct NETBAO//��������֡
{
	UCHAR mdstation[7];//Ŀ�ĵ籨��
	UCHAR sostation[7];//Դ�籨��
	UCHAR baolx;//������

	UCHAR lxm;//������

	UCHAR xinxibao[1024 * 100];
	SOCKET sock;
};

struct ATTENDTIME
{
	//QString id;
	QString weekday;//���ڼ�
	QString starttime;//��ʼʱ��
	QString endtime;//����ʱ��
};

//ѧ����
class Student
{

public:
	Student();
	Student(QString id, QString name, int sex, QString academic, QString sclass, QString figure,int in=0,int le=0,int ab=0,int to=0);

	const QString GetID() const
	{ 
		return studentid;
	}
	const QString GetName() const
	{ 
		return studentname;
	}
	const QString GetAcademic() const
	{ return studentacademic; }
	const QString GetAclass() const
	{ 
		return studentclass;
	}
	const int GetSex() const
	{ 
		return studentsex; 
	}
	const QString GetFigure() const
	{ 
		return studentfigure; 
	}
	const int GetIntimes() const
	{ 
		return intimes;
	}
	const int GetLetimes() const
	{ 
		return letimes;
	}
	const int GetAbtimes() const 
	{ 
		return abtimes; 
	}
	const int GetTotimes() const 
	{ 
		return totimes; 
	}
	void AddIntime() 
	{ 
		++intimes;
	}
	void AddLetime() 
	{ 
		++letimes; 
	}
	void AddAbtime() 
	{ 
		++abtimes;
	}
	void AddTotime()
	{ 
		++totimes;
	}
	void MinusIntime() 
	{ 
		--intimes; 
	}
	void MinusLetime()
	{ 
		--letimes; 
	}
	void MinusAbtime()
	{ 
		--abtimes; 
	}
	void MinusTotime() 
	{ 
		--totimes; 
	}
private:
	QString studentid;
	QString studentname;
	int studentsex;
	QString studentclass;
	QString studentacademic;
	QString studentfigure;
	int intimes;
	int letimes;
	int abtimes;
	int totimes;
};

//�༶��
class Aclass 
{

public:
	Aclass();
	//Aclass(const Aclass& a);
	Aclass(QString id, QString name,QString academic);
	//Aclass(QString id, QString name, QString academic, QString student);

	const QString GetID() const 
	{ 
		return classid; 
	}
	const QString GetName() const 
	{
		return classname;
	}
	const QString GetAcademic() const
	{ 
		return classacademic;
	}

private:
	QString classid;
	QString classname;
	QString classacademic;
	//QStringList classstudent;
};

//ѧԺ��
class Academic
{

public:
	Academic();
	//Academic(const Academic& a);
	Academic(QString id, QString name);
	//Academic(QString id, QString name, QString aclass);

	~Academic();

	const QString GetID() const
	{
		return academicid;
	}
	const QString GetName() const
	{ 
		return academicname;
	}

private:
	QString academicid;
	QString academicname;
	//QStringList academicclass;
};


//�û���
class User
{

public:
	User();
	User(QString id, QString psd, int identify);

	const QString GetUID() const
	{ 
		return userid; 
	}
	const QString GetPsd() const
	{ 
		return password;
	}
	const int GetIdentify() const
	{ 
		return useridentify;
	}

	void SetUID(QString id) 
	{ 
		userid = id;
	}
	void SetIdentify(int identify)
	{ 
		useridentify = identify;
	}
private:
	QString userid;
	QString password;
	int useridentify;
};

//�豸��
class Device
{
public :
	Device();
	Device(QString qname,QString qip);
	~Device();

	void SetName(QString qname);
	void SetIp(QString qip);
	QString GetName();
	QString GetIp();
private:
	QString name;
	QString ip;
};


//���ڽ����
class Result 
{

public:
	Result();
	Result(QString rid,QString id, int theory, int actual, QString absencs,QString leave,QString submit);

	void SetRID(QString ri) 
	{ 
		resultid = ri; 
	}
	void SetAID(QString id) 
	{ 
		attendanceid = id;
	}
	void SetTNum(int num) 
	{ 
		theorynumber = num;
	}
	void SetANum(int num) 
	{ 
		actualnumber = num;
	}
	void SetAStu(QString a) 
	{ 
		absencestudent = a;
	}
	void SetLStu(QString a) 
	{ 
		leavestudent = a;
	}
	void SetUID(QString a) 
	{ 
		submituser = a; 
	}

	const QString GetRID() const
	{ 
		return resultid; 
	}
	const QString GetAID() const 
	{
		return attendanceid;
	}
	const int GetTNum() const
	{ 
		return theorynumber;
	}
	const int GetANum() const 
	{ 
		return actualnumber; 
	}
	const QString GetAStu() const 
	{ 
		return absencestudent;
	}
	const QString GetLStu() const
	{ 
		return leavestudent;
	}
	const QString GetUID() const 
	{ 
		return submituser; 
	}

private:
	QString resultid;
	QString attendanceid;
	int theorynumber;
	int actualnumber;
	QString absencestudent;
	QString leavestudent;
	QString submituser;
};


//������Ϣ��
class Attendance 
{

public:
	Attendance();
	Attendance(QString id, QString mid, QString name, QString time, QString aclass);
	Attendance(QString id, QString mid, QString name, QDateTime stime, QDateTime etime, QString aclass);

	//QString TimeToString();
	//QString StudentToString();

	const QString GetID() const
	{
		return attendanceid;
	}
	const QString GetName() const
	{
		return attendancename;
	}
	const QString GetMID() const 
	{
		return attendancemachineid;
	}
	const QString GetAclass() const
	{
		return attendanceclass;
	}
	const QString GetSETime() const;

	QVector<ATTENDTIME> attendancetime;//����ʱ��
private:
	QString attendanceid;
	QString attendancemachineid;
	QString attendancename;
	QDateTime attendancestarttime;
	QDateTime attendanceendtime;
	QString attendanceclass;
	
};


//���ݹ�����
class DataManager:public QObject
{
	Q_OBJECT
public:
	explicit DataManager(QString configfile);

	//���ݿ�ĸ��£����룬ɾ����,ͨ��op��ʽ��ȷ����

	//  0������
	//	1:ɾ��
	bool AcademicOP(const Academic& a, int op);
	bool AclassOP(const Aclass& a, int op);
	bool StudentOP(const Student& s, int op);
	bool UserOP(const User& u, int op);
	bool AttendanceOP(const Attendance& a, int op);
	bool AttendanceTimeOP(QString id, QString weekday, QString starttime, QString endtime);
	bool ResultOP(const Result& a, int op);

	bool UserLogin(User user);



	//�����ݿ��и������µ����ݵ�������
	void updateAcademic();
	void updateClass();
	void updateStudent();
	void updateAttendance();
	void updateResult(User user);
	void updateUser();

	QList<Aclass> GetClassFromString(QStringList classes);
	QList<Student> GetStudentFromString(QStringList students);

	QVector<Academic>* GetAcademics() 
	{
		return academics;
	}
	QVector<Aclass>* GetAClass()
	{
		return classes;
	}
	QVector<Student>* GetStudent()
	{
		return students;
	}
	QVector<Attendance>* GetAttendance()//���ؿ�����Ϣ�б�
	{
		return attendances;
	}
	QVector<Result>* GetResult()
	{
		return results;
	}
	QVector<User>* GetUser()
	{
		return users;
	}
	QVector<Device>* GetDevice()
	{
		return devices;
	}
	TcpServer * GetTcp()
	{
		return tcp;
	}
	User GetCurrentUser()//��ǰ�û�
	{
		return currentusers;
	}
	QList<NETBAO> * GetNetBao()
	{
		return netbao;
	}

	bool FindDevic(QString name);


	CRITICAL_SECTION * GetCriNetBao()
	{
		return g_cNETBAO;
	}


private:

	//���ݿ�����
	bool InitDataBase(QString server, QString database, QString uid, QString pwd);

	//��ʼ��������Ϣ��
	//�����ݿ��е����ݸ��µ�������
	void InitAcademics();
	void InitStudents();
	void InitClasss();
	void InitResult(User user);
	void InitUser();
	void InitAttendances();
	void InitDevics();

	QVector<Academic>* academics;//ѧԺ�б�
	QVector<Aclass>* classes;//�༶�б�
	QVector<Student>* students;//ѧ���б�
	QVector<Attendance>* attendances;//�����б�
	QVector<Result>* results;//���ڽ����
	QVector<User>* users;//�û��б�
	QVector<Device>* devices;//�豸�б�
	User currentusers;//��ǰ�û�

	TcpServer * tcp;
	
	QList<NETBAO> * netbao;
	CRITICAL_SECTION *g_cNETBAO;

};

typedef struct
{
	DataManager *dm;
	SOCKET ssock;
}THREADARG;
#endif // DATAMANAGER_H
