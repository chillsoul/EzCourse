#pragma once

#include <QWidget>
#include "ui_ManagerWindow.h"
#include "SelectTeacher.h"
#pragma execution_character_set("utf-8")

class ManagerWindow : public QWidget
{
	Q_OBJECT

public:
	ManagerWindow(QWidget *parent = Q_NULLPTR);
	~ManagerWindow();
	void getCourseInfo();

private:
	Ui::ManagerWindow ui;

private slots:
	void on_pushButton_delete_clicked();
	void on_pushButton_search_clicked();
	void on_tableView_show_clicked();
	void on_pushButton_update_clicked();
	void on_pushButton_insert_clicked();
	void on_pushButton_srchTeaid_clicked();
	void receiveTeaid(QString);
};
