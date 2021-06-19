#include "DatabaseConnection.h"


DatabaseConnection::DatabaseConnection() {
	DB = QSqlDatabase::addDatabase("QSQLITE");
	DB.setDatabaseName("./EzCourse.db");
	if (!DB.open()) {
		QMessageBox::warning(0, "Database Error", DB.lastError().text());
		exit(-1);
	}
}
