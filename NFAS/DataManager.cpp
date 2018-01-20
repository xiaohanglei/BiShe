#include "DataManager.h"
#include <QtSql/QSqlDatabase>  
#include <QMessageBox>
#include <QTextCodec> 
#include <QtSql/QSqlError>
#include <QSqlQuery>  
#include <QVariant>
#include <QFile> 
#include <QTextStream> 
#include <QSettings>

Academic::Academic()
{

}

//Academic::Academic(QString id, QString name, QString aclass) :
//	academicid(id),
//	academicname(name)
//{
//	academicclass = aclass.split(",");
//}

Academic::Academic(QString id, QString name):
	academicid(id),
	academicname(name)
{

}

Academic::~Academic()
{

}

//Academic::Academic(const Academic& a)
//{
//	academicid = a.academicid;
//	academicname = a.academicname;
//	academicclass = a.academicclass;
//}

Aclass::Aclass()
{

}

Aclass::Aclass(QString id, QString name, QString academic):
	classid(id),
	classname(name),
	classacademic(academic)
{
}

//Aclass::Aclass(QString id, QString name, QString academic, QString student) :
//	classid(id),
//	classname(name),
//	classacademic(academic)
//{
//	classstudent = student.split(",");
//}

//Aclass::Aclass(const Aclass& a)
//{
//	classid = a.classid;
//	classname = a.classname;
//	classstudent = a.classstudent;
//}

Student::Student()
{

}

Student::Student(QString id, QString name, int sex, QString academic, 
	QString sclass,QString figure, int in, int le, int ab, int to):
	studentid(id),
	studentname(name),
	studentsex(sex), 
	studentclass(sclass),
	studentacademic(academic),
	studentfigure(figure),
	intimes(in),
	letimes(le),
	abtimes(ab),
	totimes(to)
{

}

User::User()
{

}

User::User(QString id, QString psd, int identify):
	userid(id),
	password(psd),
	useridentify(identify)
{

}

Result::Result()
{

}
//���
Result::Result(QString ri,QString id, int theory, int actual, QString absencs, QString leave, QString submit):
	resultid(ri),
	attendanceid(id),
	theorynumber(theory),
	actualnumber(actual),
	absencestudent(absencs),
	leavestudent(leave),
	submituser(submit)
{

}

Attendance::Attendance()
{

}

//������Ŀ
Attendance::Attendance(QString id, QString mid, QString name, QString time, QString aclass):
	attendanceid(id),
	attendancemachineid(mid),
	attendancename(name),
	attendanceclass(aclass)
{
	auto times = time.split("-");
	//���ÿ���ʱ��,ʵ��ûʲô��
	if (times.length() == 2)
	{
		attendancestarttime = QDateTime::fromString(times[0], "yyyy/MM/dd HH:mm");
		attendanceendtime = QDateTime::fromString(times[1], "yyyy/MM/dd HH:mm");
	}
}

Attendance::Attendance(QString id, QString mid, QString name, QDateTime stime, QDateTime etime, QString aclass):
	attendanceid(id),
	attendancemachineid(mid),
	attendancename(name),
	attendancestarttime(stime),
	attendanceendtime(etime),
	attendanceclass(aclass)
{
}


const QString Attendance::GetSETime() const
{
	return attendancestarttime.toString("yyyy/MM/dd HH:mm") + "-" + attendanceendtime.toString("yyyy/MM/dd HH:mm");
}


DataManager::DataManager(QString configfile)//�������ļ��ж�ȡ���ݿ�������Ϣ
{

	tcp = new TcpServer;

	QString server = "127.0.0.1";//Ĭ��������Ϣ
	QString database = "nfas";
	QString uid = "sa";
	QString pwd = "586926";

	//�����ļ�������
	QSettings *configIni = new QSettings(configfile, QSettings::IniFormat);	

	QFile config(configfile);//�ж��ļ��Ƿ����
	if (!config.exists())//����ļ�������.�Զ�����
	{
		//----------------	
		/*configIni->setValue("DBServer/ip", server);
		configIni->setValue("/DBServer/database", database);
		configIni->setValue("/DBServer/uid", uid);
		configIni->setValue("/DBServer/pwd", pwd);*/
		QMessageBox::information(0, "�����ļ�����", "�Ҳ��������ļ������������ļ���", QMessageBox::Ok);
		return;
	}
	else//��ȡ�����ļ�
	{
		//--------------		��ȡ���ݿ�������Ϣ
		server		=	configIni->value("/DBServer/ip").toString();
		database	=	configIni->value("/DBServer/database").toString();
		uid			=	configIni->value("/DBServer/uid").toString();
		pwd			=	configIni->value("/DBServer/pwd").toString();

		//--------------		��ȡTCP������������Ϣ
		QByteArray ip;
		ip = configIni->value("/TcpServer/ip").toByteArray();
		strcpy(tcp->m_IpAadress, ip.data());
		tcp->m_Port = configIni->value("/TcpServer/port").toInt();
	
		

	}
	delete configIni;

	//�½���������󱣴�����	
	tcp->LoadWinSock();//����tcp����
	tcp->StartServer();


	//ѧԺ��ѧ�����༶�����ڡ��û������ڽ��
	academics = new QVector<Academic>;
	students = new QVector<Student>;
	classes = new QVector<Aclass>;
	attendances = new QVector<Attendance>;
	users = new QVector<User>;
	results = new QVector<Result>;
	devices = new QVector<Device>;

	if (!InitDataBase(server, database, uid, pwd)) //���ݿ�����
	{
		exit(-1);
	}	
}

//���ݿ�����
bool DataManager::InitDataBase(QString server,QString database,QString uid,QString pwd)
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
	db.setDatabaseName(QString("DRIVER={SQL SERVER};"
		"SERVER=%1;"
		"DATABASE=%2;"
		"UID=%3;"
		"PWD=%4;").arg(server)
		.arg(database)
		.arg(uid)
		.arg(pwd));
	if (!db.open())
	{
		//��������ʧ�ܣ�������sqlserver�Ĵ�����ʾ
		QMessageBox::warning(0, tr("Cannot open database"),
			db.lastError().databaseText(), QMessageBox::Cancel);
		return false;
	}
	//��ʼ�����������б�
	//���ȴ����ݿ��в�ѯ��������Ϣ�����ص�������
	InitAcademics();
	InitStudents();
	InitClasss();
	InitAttendances();

	//InitResult();

	InitUser();
	InitDevics();
	return true;
}

bool DataManager::AcademicOP(const Academic& a, int op)
{
	QSqlQuery query;
	switch (op)
	{
	case 0:
		query.prepare("INSERT INTO academictable (academicid,academicname) VALUES (?,?)");
		query.bindValue(0, a.GetID());
		query.bindValue(1, a.GetName());
		break;
	case 1:
		query.prepare("DELETE FROM academictable WHERE academicid=?");
		query.bindValue(0, a.GetID());
		break;
	default:
		break;
	}
	return query.exec();
}

bool DataManager::AclassOP(const Aclass& a, int op)
{
	QSqlQuery query1;
	switch (op)
	{
	case 0:
		query1.prepare("INSERT INTO classtable (classid,classname,classacademic) VALUES (?,?,?)");
		query1.bindValue(0, a.GetID());
		query1.bindValue(1, a.GetName());
		query1.bindValue(2, a.GetAcademic());
		break;
	case 1:
		query1.prepare("UPDATE classtable SET classname=?,classacademic=? WHERE classid=?");
		query1.bindValue(0, a.GetName());
		query1.bindValue(1, a.GetAcademic());
		query1.bindValue(2, a.GetID());
		break;
	case 2:
		query1.prepare("DELETE FROM classtable WHERE classid=?");
		query1.bindValue(0, a.GetID());
		break;
	default:
		break;
	}
	return query1.exec();
}

bool DataManager::StudentOP(const Student& s, int op)
{
	QSqlQuery query1;
	switch (op)
	{
	case 0:
		query1.prepare("INSERT INTO studenttable (studentid,studentname,studentsex,studentacademic,studentclass,studentfigure) VALUES (?,?,?,?,?,?)");
		query1.bindValue(0, s.GetID());
		query1.bindValue(1, s.GetName());
		query1.bindValue(2, s.GetSex());
		query1.bindValue(3, s.GetAcademic());
		query1.bindValue(4, s.GetAclass());
		query1.bindValue(5, s.GetFigure());
		break;
	case 1://�޸�ѧ����Ϣ
		query1.prepare("UPDATE studenttable SET studentname=?,studentsex=?,studentacademic=?,studentclass=? WHERE studentid=?");
		query1.bindValue(0, s.GetName());
		query1.bindValue(1, s.GetSex());
		query1.bindValue(2, s.GetAcademic());
		query1.bindValue(3, s.GetAclass());
		query1.bindValue(4, s.GetID());
		break;
	case 2:
		query1.prepare("DELETE FROM studenttable WHERE studentid=?");
		query1.bindValue(0, s.GetID());
		break;
	case 3://�޸�ǩ��������Ӧ������
		query1.prepare("UPDATE studenttable SET intimes=?,totimes=? WHERE studentid=?");
		query1.bindValue(0, s.GetIntimes());
		query1.bindValue(1, s.GetTotimes());
		query1.bindValue(2, s.GetID());
		break;
	case 4://�޸���ٴ�����Ӧ������
		query1.prepare("UPDATE studenttable SET letimes=?,totimes=? WHERE studentid=?");
		query1.bindValue(0, s.GetLetimes());
		query1.bindValue(1, s.GetTotimes());
		query1.bindValue(2, s.GetID());
		break;
	case 5://�޸�ȱ�ڴ�����Ӧ������
		query1.prepare("UPDATE studenttable SET abtimes=?,totimes=? WHERE studentid=?");
		query1.bindValue(0, s.GetAbtimes());
		query1.bindValue(1, s.GetTotimes());
		query1.bindValue(2, s.GetID());
		break;
	case 6://�޸ĸ�ѧ���������
		query1.prepare("UPDATE studenttable SET intimes=?,letimes=?,abtimes=?,totimes=? WHERE studentid=?");
		query1.bindValue(0, s.GetIntimes());
		query1.bindValue(1, s.GetLetimes());
		query1.bindValue(2, s.GetAbtimes());
		query1.bindValue(3, s.GetTotimes());
		query1.bindValue(4, s.GetID());
		break;
	default:
		break;
	}
	return query1.exec();
}

bool DataManager::UserOP(const User& u, int op)
{
	QSqlQuery query;
	switch (op)
	{
	case 0:
		query.prepare("INSERT INTO admintable (userid,userpassword,useridentify) VALUES (?,?,?)");
		query.bindValue(0, u.GetUID());
		query.bindValue(1, u.GetPsd());
		query.bindValue(2, u.GetIdentify());
		break;
	case 1:
		query.prepare("UPDATE admintable SET useridentify=? WHERE userid=?");
		query.bindValue(0, u.GetIdentify());
		query.bindValue(1, u.GetUID());
		break;
	case 2:
		query.prepare("DELETE FROM admintable WHERE userid=?");
		query.bindValue(0, u.GetUID());
		break;
	default:
		break;
	}
	return query.exec();
}

bool DataManager::AttendanceOP(const Attendance& a, int op)
{
	QSqlQuery query;
	switch (op)
	{
	case 0:
		query.prepare("INSERT INTO attendancetable (attendanceid,attendancemachineid,attendancename,attendancetime,attendanceclass) VALUES (?,?,?,?,?)");
		query.bindValue(0, a.GetID());
		query.bindValue(1, a.GetMID());
		query.bindValue(2, a.GetName());
		query.bindValue(3, a.GetSETime());
		query.bindValue(4, a.GetAclass());
		break;
	case 1:
		query.prepare("UPDATE attendancetable SET attendanceclass=? WHERE attendanceid=?");
		query.bindValue(0, a.GetAclass());
		query.bindValue(1, a.GetID());
		break;
	case 2:
		query.prepare("DELETE FROM attendancetable WHERE attendanceid=?");
		query.bindValue(0, a.GetID());
		break;
	default:
		break;
	}
	return query.exec() /*&& ResultOP(Result(a.GetRID(), 0, 0,"","",""), 2)*/;
}

bool DataManager::ResultOP(const Result& a, int op)
{
	QSqlQuery query;
	switch (op)
	{
	case 0:
		query.prepare("INSERT INTO resulttable (resultid,attendanceid,actualnumber,theorynumber,absencestudent,leavestudent,submituser) VALUES (?,?,?,?,?,?,?)");
		query.bindValue(0, a.GetRID());
		query.bindValue(1, a.GetAID());
		query.bindValue(2, a.GetANum());
		query.bindValue(3, a.GetTNum());
		query.bindValue(4, a.GetAStu());
		query.bindValue(5, a.GetLStu());
		query.bindValue(6, a.GetUID());
		break;
	case 1:
		query.prepare("UPDATE resulttable SET theorynumber=?,actualnumber=?,absencestudent=?,leavestudent=? WHERE resultid=? AND attendanceid=?");
		query.bindValue(0, a.GetTNum());
		query.bindValue(1, a.GetANum());
		query.bindValue(2, a.GetAStu());
		query.bindValue(3, a.GetLStu());
		query.bindValue(4, a.GetRID());
		query.bindValue(5, a.GetAID());
		break;
	case 2:
		if (a.GetRID() == "") {
			query.prepare("DELETE FROM resulttable WHERE attendanceid=?");
			query.bindValue(0, a.GetAID());
		}
		else {
			query.prepare("DELETE FROM resulttable WHERE resultid=?");
			query.bindValue(0, a.GetRID());
		}
		break;
	default:
		break;
	}
	return query.exec();
}

void DataManager::updateAcademic()
{
	academics->clear();
	InitAcademics();
}

void DataManager::updateClass()
{
	classes->clear();
	InitClasss();
}

void DataManager::updateStudent()
{
	students->clear();
	InitStudents();
}

void DataManager::updateAttendance()
{
	attendances->clear();
	InitAttendances();
}

void DataManager::updateResult(User user)
{
	results->clear();
	InitResult(user);
}

void DataManager::updateUser()
{
	users->clear();
	InitUser();
}

QList<Aclass> DataManager::GetClassFromString(QStringList classes)
{
	return QList<Aclass>();
}

QList<Student> DataManager::GetStudentFromString(QStringList students)
{
	return QList<Student>();
}

void DataManager::InitAcademics()
{
	QSqlQuery query("SELECT * FROM academictable");
	while (query.next())
	{
		QString id = query.value(0).toString();
		QString name = query.value(1).toString();
		//QString aclass = query.value(2).toString();
		this->academics->append(Academic(id, name/*, aclass*/));
	}
}

void DataManager::InitStudents()
{
	QSqlQuery query("SELECT * FROM studenttable");
	while (query.next()) 
	{
		QString id = query.value(0).toString();
		QString name = query.value(1).toString();
		int sex = query.value(2).toInt();
		QString academic = query.value(4).toString();
		QString aclass = query.value(3).toString();
		QString figure = query.value(5).toString();
		int in = query.value(6).toInt();
		int le = query.value(7).toInt();
		int ab = query.value(8).toInt();
		int to = query.value(9).toInt();
		this->students->append(Student(id, name,sex,academic,aclass,figure,in,le,ab,to));
	}
}

void DataManager::InitClasss()
{
	QSqlQuery query("SELECT * FROM classtable");
	while (query.next()) 
	{
		QString id = query.value(0).toString();
		QString name = query.value(1).toString();
		QString student = query.value(2).toString();
		classes->append(Aclass(id, name, student));
	}
}

void DataManager::InitResult(User user)
{
	QSqlQuery query("SELECT * FROM resulttable");
	while (query.next()) 
	{
		QString rid = query.value(0).toString();
		QString id = query.value(1).toString();
		int tnum = query.value(2).toInt();
		int anum = query.value(3).toInt();
		QString astu = query.value(4).toString();
		QString lstu = query.value(5).toString();
		QString uid = query.value(6).toString();

		//���Ȩ��Ϊ 0����Ա�Ļ��������еĿ��ڽ������ӵ�������
		if (user.GetIdentify() == 0)
		{
			results->append(Result(rid,id, tnum, anum, astu, lstu, uid));
		}
		else//���Ϊ 1 ��ʦ�Ļ���ֻ�ǽ��Լ��ύ�Ľ������ӵ������Ϣ��
		{
			if (uid == user.GetUID())
			{
				results->append(Result(rid,id, tnum, anum, astu, lstu, uid));
			}
		}
	}
}

void DataManager::InitUser()
{
	QSqlQuery query("SELECT * FROM admintable");
	while (query.next()) 
	{
		QString id = query.value(0).toString();
		int identify = query.value(2).toInt();
		users->append(User(id, "", identify));
	}
}

bool DataManager::UserLogin(User user)
{
	QSqlQuery query(QString("SELECT * FROM admintable WHERE userid='%1' AND userpassword='%2'")
		.arg(user.GetUID())
		.arg(user.GetPsd()));
	//���ƥ�䵽���û���������Ϊ��ǰ�����û�
	while (query.next())
	{
		currentusers.SetUID(query.value(0).toString());
		currentusers.SetIdentify(query.value(2).toInt());
		return true;
	}
	return false;
}

void DataManager::InitAttendances()
{
	QSqlQuery query("SELECT * FROM attendancetable");
	while (query.next())
	{
		QString id = query.value(0).toString();
		QString mid = query.value(1).toString();
		QString name = query.value(2).toString();
		QString time = query.value(3).toString();
		QString aclass = query.value(4).toString();
		attendances->append(Attendance(id, mid, name, time, aclass));
	}
}

void DataManager::InitDevics()
{
	//�����ļ�������
	QSettings *configIni = new QSettings("config.ini", QSettings::IniFormat);

	//��ȡ�����ļ������еĿͻ����б�
	//ALLCLIENTLIST tempClient;

	QString tempQS;
	QByteArray tempQB;
	int sum = 0;
	sum = configIni->value("/ClientList/sum").toInt();
	for (int i = 0; i < sum; i++)
	{
		tempQS = configIni->value("/ClientList/" + QString::number(i, 10)).toString();	
		
		devices->append(Device(tempQS.split("\t").first(), tempQS.split("\t").last()));
	}
	delete configIni;
}

Device::Device()
{

}

Device::Device(QString qname, QString qip):name(qname),ip(qip)
{

}

Device::~Device()
{
}

void Device::SetName(QString qname)
{
	name = qname;
}

void Device::SetIp(QString qip)
{
	ip = qip;
}

QString Device::GetName()
{
	return name;
}

QString Device::GetIp()
{
	return ip;
}
