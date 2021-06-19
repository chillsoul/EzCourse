#include "SelectTeacher.h"
#include <QMessageBox>
#include <QDebug>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

SelectTeacher::SelectTeacher(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	on_pushButton_search_clicked();
}

SelectTeacher::~SelectTeacher()
{
	qDebug()<<"~SelectTeacher() called"<<endl;
}

void SelectTeacher::on_pushButton_search_clicked()
{
	//qDebug() << ui.tableView_show->model() << endl;
	//delete ui.tableView_show->model();//debug发现此处删除无用，因为每次双击/关闭时该指针分配的空间已取消

	QString inputText = ui.lineEdit_search->text();
	QString sql = "select teaid as 教师ID,teaname as 姓名 from teacher where teaid like '%%1%' or teaname like '%%1%'";
	QSqlQueryModel* model = new QSqlQueryModel(this);
	model->setQuery(sql.arg(inputText));

	ui.tableView_show->setModel(model);
	ui.tableView_show->setColumnWidth(0, 80);
}

void SelectTeacher::on_tableView_show_doubleClicked()
{
	int row = ui.tableView_show->currentIndex().row();
	QModelIndex index = ui.tableView_show->model()->index(row, 0);
	QString teaid=index.data().toString();
	emit sendTeaid(teaid);
	disconnect(this, SIGNAL(sendTeaid(QString)), 0, 0);
	close();
}
