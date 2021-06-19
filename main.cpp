#include "LoginWindow.h"
#include <QtWidgets/QApplication>
#include "DatabaseConnection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DatabaseConnection db;
    LoginWindow w;
    w.show();
    return a.exec();
}
