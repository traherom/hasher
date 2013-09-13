#ifndef FILELISTWINDOW_H
#define FILELISTWINDOW_H

#include "hashdatabase.h"

#include <QMainWindow>

namespace Ui {
class FileListWindow;
}

class FileListWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FileListWindow(QWidget *parent = 0);
    ~FileListWindow();

private slots:
    void on_createButton_clicked();

private:
    void createDatabase();
    void openDatabase(HashDatabase *alreadyOpenDb);
    void openDatabase(QString dbPath);
    void closeDatabase();

    Ui::FileListWindow *ui;

    HashDatabase *db;
};

#endif // FILELISTWINDOW_H
