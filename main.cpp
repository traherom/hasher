#include <QApplication>
#include <QPushButton>
#include <sqlite3.h>

#include "filelistwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    FileListWindow mainWin;
    mainWin.show();

    return a.exec();
}
