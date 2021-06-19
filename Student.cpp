#include "Student.h"

Student::Student(int id,QString name,QString major)
{
	this->id = id;
	this->name = name;
	this->major = major;
}

Student::Student()
{

}

QString Student::getName()
{
	return name;
}

int Student::getId()
{
	return id;
}

QString Student::getMajor(){
	return major;
}