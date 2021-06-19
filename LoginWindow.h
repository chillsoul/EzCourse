#pragma once

#include <QtWidgets/QMainWindow>
#include <QtSql/QSqlQueryModel>
#include "ui_LoginWindow.h"
#include "Student.h"

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = Q_NULLPTR);

private:
    Ui::LoginWindow ui;
signals:
    void loggedin(Student stu);
private slots:
    void on_pushButton_login_clicked();
};
