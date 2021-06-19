#pragma once

#include <QWidget>
#include "ui_StudentWindow.h"
#include "Student.h"
#pragma execution_character_set("utf-8")

class StudentWindow : public QWidget
{
	Q_OBJECT

public:
	StudentWindow(QWidget *parent = Q_NULLPTR);
	~StudentWindow();

private:
	Ui::StudentWindow ui;
	Student stu;
	void getCourseInfo();

private slots:
	void slot_receiveStuInfo(Student stu);
	void on_pushButton_search_clicked();
	void on_pushButton_quitCourse_clicked();
	void on_pushButton_chooseCourse_clicked();
};
