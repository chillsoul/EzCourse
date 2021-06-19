#include "StudentWindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QAbstractProxyModel>
#include <QSortFilterProxyModel>

StudentWindow::StudentWindow(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

StudentWindow::~StudentWindow()
{
}

void StudentWindow::getCourseInfo()
{
	delete ui.tableView_show->model();
	QString sql = "select course.cozid as 课号,course.cozname as 课程,teacher.teaname as 教师,course.cozcredit as 学分 from stucourse,course,teacher where stucourse.stuid='%1' and course.cozid=stucourse.cozid and course.teaid=teacher.teaid order by course.cozid asc";
	qDebug() << sql.arg(stu.getId()) << endl;

	QSortFilterProxyModel* proxy = new QSortFilterProxyModel(this);
	QSqlQueryModel* model = new QSqlQueryModel(proxy);
	model->setQuery(sql.arg(stu.getId()));
	proxy->setSourceModel(model);

	ui.tableView_show->setModel(proxy);
	ui.tableView_show->sortByColumn(0,Qt::AscendingOrder);
	ui.tableView_show->setColumnWidth(0, 40);
	ui.tableView_show->setColumnWidth(2, 100);
	ui.tableView_show->setColumnWidth(3, 40);
}

void StudentWindow::slot_receiveStuInfo(Student stu)
{
	this->stu = stu;
	QString welcome = "欢迎使用EzCourse简易公选课系统，当前用户来自 %1 专业的 %2 同学 学号 %3";
	ui.label_welcome->setText(welcome.arg(stu.getMajor()).arg(stu.getName()).arg(stu.getId()));
	//QMessageBox::about(0, "提示", "StuWindow收到信号！");
	getCourseInfo();
}

void StudentWindow::on_pushButton_search_clicked()
{
	delete ui.tableView_searchResult->model();
	QString inputText = ui.lineEdit_search->text();
	QString sql = "select course.cozid as 课号,course.cozname as 课程,teacher.teaname as 教师,course.cozcredit as 学分,course.stumax-course.stunum as 余量 from course,teacher where course.teaid=teacher.teaid and (course.cozname like '%%1%' or teacher.teaname like '%%1%' or course.cozid like '%%1%')";
	qDebug() << sql.arg(inputText) << endl;

	QSortFilterProxyModel *proxy = new QSortFilterProxyModel();
	QSqlQueryModel* model = new QSqlQueryModel(proxy);
	model->setQuery(sql.arg(inputText));
	proxy->setSourceModel(model);
	ui.tableView_searchResult->setModel(proxy);
	ui.tableView_show->sortByColumn(0, Qt::AscendingOrder);
	ui.tableView_searchResult->setColumnWidth(0, 40);
	ui.tableView_searchResult->setColumnWidth(2, 60);
	ui.tableView_searchResult->setColumnWidth(3, 40);
	ui.tableView_searchResult->setColumnWidth(4, 40);
}

void StudentWindow::on_pushButton_quitCourse_clicked()
{
	int row=ui.tableView_show->currentIndex().row();
	if(row==-1){
		QMessageBox::warning(0, "提示", "请先选中课程！");
		return;
	}
	QModelIndex index = ui.tableView_show->model()->index(row,0);
	QString sql = "delete from stucourse where cozid='%1' and stuid='%2'";
	QSqlQuery query;
	if(query.exec(sql.arg(index.data().toString()).arg(stu.getId()))){
		sql = "update course set stunum=stunum-1 where cozid=%1";
		query.exec(sql.arg(index.data().toString()));
		QMessageBox::about(0, "提示", "退选成功！");
		on_pushButton_search_clicked();
	}else{
		QMessageBox::warning(0, "提示", query.lastError().text());
		return;
	}
	getCourseInfo();
}

void StudentWindow::on_pushButton_chooseCourse_clicked()
{
	int row = ui.tableView_searchResult->currentIndex().row();
	if (row == -1) {
		QMessageBox::warning(0, "提示", "请先选中课程！");
		return;
	}
	QModelIndex index = ui.tableView_searchResult->model()->index(row, 0);//获取课程ID
	QSqlQuery query;
	QString sql = "select count(*) from stucourse where stuid='%1' and cozid='%2'";
	query.exec(sql.arg(stu.getId()).arg(index.data().toString()));
	query.next();
	if(query.value(0).toInt()>0){
		QMessageBox::warning(0, "提示", "您已选过该课程！");
		return;
	}

	index = ui.tableView_searchResult->model()->index(row, 4);
	if (index.data().toInt() <= 0) {
		QMessageBox::warning(0, "提示", "该课程已无余量！！");
		return;
	}

	index = ui.tableView_searchResult->model()->index(row, 0);
	sql = "insert into stucourse values(null,'%1','%2')";
	qDebug() << sql.arg(stu.getId()).arg(index.data().toString()) << endl;
	if(query.exec(sql.arg(stu.getId()).arg(index.data().toString()))){
		sql = "update course set stunum=stunum+1 where cozid=%1";
		if (query.exec(sql.arg(index.data().toString()))) {
			QMessageBox::about(0, "提示", "选课成功！");
			on_pushButton_search_clicked();
		}
		else{
			QMessageBox::warning(0, "提示", query.lastError().text());
		}
	}else{
		QMessageBox::warning(0, "提示", query.lastError().text());
		return;
	}
	getCourseInfo();
}