#ifndef FILELISTWINDOW_H
#define FILELISTWINDOW_H

#include "hashdatabase.h"

#include <boost/shared_ptr.hpp>

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

    void updateUISidebar();

private slots:
    void on_createButton_clicked();
    void on_openButton_clicked();

    void on_nsrlAddUpdateButton_clicked();

    void on_dbControlDock_visibilityChanged(bool visible);

    void on_scanNowButton_clicked();

private:
    void createDatabasePrompt();
    void openDatabasePrompt();

    void openDatabase(boost::shared_ptr<HashDatabase> alreadyOpenDb);
    void openDatabase(QString dbPath);
    void closeDatabase();

    Ui::FileListWindow *ui;

    boost::shared_ptr<HashDatabase> db;
};

#endif // FILELISTWINDOW_H
