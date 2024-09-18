#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <mysql.h>

using namespace std;

class Student 
{
public:
	string mName;
	string mAge;
	string mScore;
	 
	Student()
	{
		mName = "";
		mAge = "";
		mScore = "";
	}

	Student(string name, string age, string score)
	{
		mName = name;
		mAge = age;
		mScore = score;
	}

	Student& operator=(const Student& s)
	{
		mName = s.mName;
		mAge = s.mAge;
		mScore = s.mScore;
		return *this;
	}
};

struct StuNode
{
	Student data;
	StuNode* next;
};

StuNode* createStudentNode(Student stu)
{
	StuNode* sn = new StuNode;
	sn->data = stu;
	sn->next = nullptr;
	return sn;
}

void showAllStu(MYSQL& mysql)
{
	MYSQL_RES* mysql_res;
	MYSQL_ROW mysql_row;

	int ret = mysql_query(&mysql, "select * from student");

	mysql_res = mysql_store_result(&mysql);

	cout << "id\tname\tsex\tclass" << endl;
	cout << "-----------------------------" << endl;
	while (mysql_row = mysql_fetch_row(mysql_res))
	{
		cout << mysql_row[0] << "\t" << mysql_row[1] << "\t";
		cout << mysql_row[2] << "\t" << mysql_row[3] << endl;
	}
}

void addStu(MYSQL& mysql)
{
	string stunum;
	cout << "������ѧ�ţ�";
	cin >> stunum;

	string name;
	cout << "������������";
	cin >> name;

	string sex;
	cout << "�������Ա�";
	cin >> sex;

	string classid;
	cout << "������༶��";
	cin >> classid;

	string queryString = "INSERT INTO `school`.`student` VALUES ("
		+ stunum + ",\'" 
		+ name + "\',\'" 
		+ sex + "\'," 
		+ classid + ");";

	//cout << queryString.c_str() << endl;

	int ret = mysql_query(&mysql, queryString.c_str());
	switch (ret)
	{
	case 0:
		cout << "����ɹ���" << endl;
		break;
	case 1:
		cout << "����ʧ�ܣ�����ԭ��" << mysql_errno(&mysql) << endl;
		break;
	default:
		break;
	}
}

void deleteStu(MYSQL& mysql)
{
	string id;
	cout << "������Ҫɾ��ѧ����id��";
	cin >> id;

	string queryString = "DELETE FROM `school`.`student` WHERE ID = "
		+ id;

	int ret = mysql_query(&mysql, queryString.c_str());
	switch (ret)
	{
	case 0:
		cout << "ɾ���ɹ���" << endl;
		break;
	case 1:
		cout << "ɾ��ʧ�ܣ�����ԭ��" << mysql_errno(&mysql) << endl;
		break;
	default:
		break;
	}
}

void changeStu(MYSQL& mysql)
{
	string id;
	cout << "������Ҫ�޸ĵ�ѧ����id��";
	cin >> id;

	string queryString = "UPDATE `school`.`student` SET ";

	string newname = "";
	string newclass = "";
	char selection;

	cout << "�Ƿ�Ҫ�޸����֣�(Y/N) ";
	cin >> selection;
	if (selection == 'Y' || selection == 'y')
	{
		cout << "�����������֣�";
		cin >> newname;
	}

	cout << "�Ƿ�Ҫ�޸İ༶��(Y/N) ";
	cin >> selection;
	if (selection == 'Y' || selection == 'y')
	{
		cout << "�������°༶��";
		cin >> newclass;
	}


	if (newname == "" && newclass == "")
	{
		cout << "δ���κ��޸�" << endl;
		return;
	}

	if (newname != "")
	{
		queryString += "name = " + newname;
	}

	if (newclass != "")
	{
		if (newname != "")
			queryString += ",";
		queryString += "class_id = " + newclass;
	}

	queryString += " where id = " + id;

	cout << queryString << endl;

	int ret = mysql_query(&mysql, queryString.c_str());
	switch (ret)
	{
	case 0:
		cout << "�޸ĳɹ���" << endl;
		break;
	case 1:
		cout << "�޸�ʧ�ܣ�����ԭ��" << mysql_errno(&mysql) << endl;
		break;
	default:
		break;
	}
}

void menu(MYSQL& mysql)
{
	cout << "1��������" << endl;
	cout << "2����" << endl;
	cout << "3��ɾ" << endl;
	cout << "4����" << endl;
	cout << "5���˳�" << endl;

	int n;
	while (1)
	{
		cin >> n;

		switch (n)
		{
		case 1:
			showAllStu(mysql);
			break;
		case 2:
			addStu(mysql);
			break;
		case 3:
			deleteStu(mysql);
			break;
		case 4:
			changeStu(mysql);
			break;
		case 5:
			exit(1);
		default:
			break;
		}
	}
}

int main()
{
	//menu();
	MYSQL mysql;

	mysql_init(&mysql);
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");

	if (mysql_real_connect(&mysql, "localhost", "root", "Kns123456", "school", 3306,
		NULL, 0) == NULL)
	{
		cout << "����ʧ�ܣ�����ԭ��" << mysql_errno(&mysql);
		exit(-1);
	}

	menu(mysql);

	return 1;
}