#include <QCoreApplication>
#include <sqlite3.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    sqlite3 *db = NULL;
    sqlite3_open("test.db", &db);


    sqlite3_close(db);

    return a.exec();
}
