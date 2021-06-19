#pragma once

#include <QWidget>
#include "ui_SelectTeacher.h"
#pragma execution_character_set("utf-8")

class SelectTeacher : public QWidget
{
	Q_OBJECT

public:
	SelectTeacher(QWidget *parent = Q_NULLPTR);
	~SelectTeacher();

private:
	Ui::SelectTeacher ui;

signals:
	void sendTeaid(QString teaid);

private slots:
	void on_pushButton_search_clicked();
	void on_tableView_show_doubleClicked();
};
