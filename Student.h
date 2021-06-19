#pragma once
#include <QString>
using namespace std;

class Student
{
private:
	QString name;
	int id;
	QString major;
public:
	Student(int id, QString name,QString major);
	QString getName();
	int getId();
	QString getMajor();
	Student();
};

