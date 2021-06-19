#pragma once

#include <QtWidgets/QMessageBox>
#include <QString>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlError>

using namespace std;
class DatabaseConnection {
private:
	QSqlDatabase DB;
public:
	DatabaseConnection();
};