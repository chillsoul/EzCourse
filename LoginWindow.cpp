#include "LoginWindow.h"
#include <QtWidgets/QMessageBox>
#include "Student.h"
#include "StudentWindow.h"
#include "ManagerWindow.h"
#pragma execution_character_set("utf-8")


LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

void LoginWindow::on_pushButton_login_clicked()
{
    QString inputId = ui.lineEdit_usrid->text();
    QString inputPw = ui.lineEdit_usrpw->text();
    QSqlQueryModel* model=new QSqlQueryModel;
    QModelIndex index;
    int usrType = 0;
    if (ui.radioButton_stu->isChecked())
    {
        QString sql = "select count(*),stuname,stumajor from student where stuid='" + inputId + "' and stupw='" + inputPw + "'";
        model->setQuery(sql);
        usrType = 1;
	}else{
		QString sql = "select count(*) from manager where mgrid='" + inputId + "' and mgrpw='" + inputPw + "'";
		model->setQuery(sql);
        usrType = 2;
    }
    
    index= model->index(0, 0);
	if (index.data().toInt() != 0) {
		QMessageBox::about(0, "提示", "登陆成功！");
        if (usrType == 1) {
            Student stu(inputId.toInt(), model->index(0, 1).data().toString(), model->index(0, 2).data().toString());
            StudentWindow *sw=new StudentWindow;
            connect(this, SIGNAL(loggedin(Student)), sw, SLOT(slot_receiveStuInfo(Student)));
            sw->show();
            emit loggedin(stu);
            close();
        }else {
            ManagerWindow *mw=new ManagerWindow;
            mw->show();
            close();
        }
	}
	else {
		QMessageBox::about(0, "提示", "用户名或密码错误！");
	}
}
