#include "ManagerWindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QSortFilterProxyModel>
ManagerWindow::ManagerWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	getCourseInfo();
}

ManagerWindow::~ManagerWindow()
{

}

void ManagerWindow::getCourseInfo()
{
	delete ui.tableView_show->model();
	QString inputText = ui.lineEdit_search->text();
	QString sql = "select course.cozid as 课号,course.cozname as 课程,course.cozcredit as 学分,teacher.teaname as 教师,teacher.teaid as 教师ID,course.stunum as 人数,course.stumax as 容量 from course,teacher where course.teaid=teacher.teaid and (course.cozname like '%%1%' or teacher.teaname like '%%1%' or course.cozid like '%%1%') order by course.cozid asc";
	
	QSortFilterProxyModel* proxy = new QSortFilterProxyModel(this);
	QSqlQueryModel* model = new QSqlQueryModel(proxy);
	model->setQuery(sql.arg(inputText));
	proxy->setSourceModel(model);

	ui.tableView_show->setModel(proxy);
	ui.tableView_show->sortByColumn(0, Qt::AscendingOrder);
	ui.tableView_show->setColumnWidth(0, 40);
	ui.tableView_show->setColumnWidth(2, 40);
	ui.tableView_show->setColumnWidth(3, 80);
	ui.tableView_show->setColumnHidden(4, true);
	ui.tableView_show->setColumnWidth(5, 40);
	ui.tableView_show->setColumnWidth(6, 40);
}


void ManagerWindow::on_pushButton_search_clicked(){
	getCourseInfo();
}

void ManagerWindow::on_tableView_show_clicked()
{
	int row = ui.tableView_show->currentIndex().row();
	QModelIndex index = ui.tableView_show->model()->index(row, 1);
	ui.lineEdit_cozname->setText(index.data().toString());
	index = ui.tableView_show->model()->index(row, 2);
	ui.lineEdit_cozCredit->setText(index.data().toString());
	index = ui.tableView_show->model()->index(row, 4);
	ui.lineEdit_teaid->setText(index.data().toString());
	index = ui.tableView_show->model()->index(row, 6);
	ui.lineEdit_stumax->setText(index.data().toString());
}

void ManagerWindow::on_pushButton_update_clicked()
{
	int row = ui.tableView_show->currentIndex().row();
	QModelIndex index = ui.tableView_show->model()->index(row, 0);//课程号
	QString selectedCozid = index.data().toString();

	QString inputCozname = ui.lineEdit_cozname->text();
	QString inputTeaid = ui.lineEdit_teaid->text();
	QString inputStumax = ui.lineEdit_stumax->text();
	QString inputCozCredit = ui.lineEdit_cozCredit->text();
	if (ui.tableView_show->model()->index(row, 5).data().toInt()>inputStumax.toInt()){
		QMessageBox::warning(0, "提示", "不能修改为少于已选人数！！");
		return;
	}
	QString sql = "update course set cozname='%1',teaid='%2',stumax=%3,cozcredit=%4 where cozid='%5'";
	QSqlQuery query;
	qDebug() << sql.arg(inputCozname).arg(inputTeaid).arg(inputStumax).arg(selectedCozid) << endl;
	if(query.exec(sql.arg(inputCozname).arg(inputTeaid).arg(inputStumax).arg(inputCozCredit).arg(selectedCozid))){
		QMessageBox::about(0, "提示", "修改成功！");
		ui.lineEdit_cozname->setText("");
		ui.lineEdit_teaid->setText("");
		ui.lineEdit_stumax->setText("");
		ui.lineEdit_cozCredit->setText("");
		getCourseInfo();
	}else{
		QMessageBox::warning(0, "提示", query.lastError().text());
		return;
	}

}

void ManagerWindow::on_pushButton_insert_clicked()
{
	QString inputCozname = ui.lineEdit_cozname->text();
	QString inputTeaid = ui.lineEdit_teaid->text();
	QString inputStumax = ui.lineEdit_stumax->text();
	QString inputCozCredit = ui.lineEdit_cozCredit->text();
	QString sql = "select count(*) from course where cozname='%1' and teaid='%2'";
	QSqlQuery query;
	query.exec(sql.arg(inputCozname).arg(inputTeaid));
	query.next();
	if (query.value(0) > 0) {
		QMessageBox::warning(0, "提示", "请勿重复添加课程！");
		return;
	}

	sql = "insert into course values(null,'%1','%2',0,'%3',%4)";
	qDebug() << sql.arg(inputCozname).arg(inputTeaid).arg(inputStumax) << endl;
	if(query.exec(sql.arg(inputCozname).arg(inputTeaid).arg(inputStumax).arg(inputCozCredit))){
		QMessageBox::about(0, "提示", "添加成功！");
		getCourseInfo();
	}else{
		QMessageBox::warning(0, "提示", query.lastError().text());
	}
}

void ManagerWindow::on_pushButton_srchTeaid_clicked()
{
	SelectTeacher *st=new SelectTeacher;
	st->setAttribute(Qt::WA_DeleteOnClose);
	connect(st, SIGNAL(sendTeaid(QString)), this, SLOT(receiveTeaid(QString)));
	st->show();
}

void ManagerWindow::receiveTeaid(QString teaid)
{
	ui.lineEdit_teaid->setText(teaid);
}

void ManagerWindow::on_pushButton_delete_clicked(){
	int row = ui.tableView_show->currentIndex().row();
	if (row == -1) {
		QMessageBox::warning(0, "提示", "请先选中课程！");
		return;
	}
	QModelIndex index = ui.tableView_show->model()->index(row, 5);
	if(index.data().toInt()>0){
		QMessageBox::warning(0, "提示", "该课程有学生已选！请先通知其退选！！！");
		return;
	}

	index = ui.tableView_show->model()->index(row, 0);
	QString sql = "delete from course where cozid=%1";
	QSqlQuery query;
	if (query.exec(sql.arg(index.data().toString()))) {
		QMessageBox::about(0, "提示", "删除课程成功！");
		ui.lineEdit_search->setText("");
		getCourseInfo();
	}
	else {
		QMessageBox::warning(0, "提示", query.lastError().text());
		return;
	}
}
