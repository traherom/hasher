#include "filelistwindow.h"
#include "ui_filelistwindow.h"

#include "hashdatabase.h"

#include <iostream>

#include <QFileDialog>
#include <QString>

FileListWindow::FileListWindow(QWidget *parent) :
    QMainWindow(parent),
    db(NULL),
    ui(new Ui::FileListWindow)
{
    ui->setupUi(this);
}

FileListWindow::~FileListWindow()
{
    if(db) {
        delete db;
    }

    delete ui;
}

void FileListWindow::on_createButton_clicked()
{
    createDatabase();
}

void FileListWindow::createDatabase()
{
    // Open dialog to allow user to select where to save database
    QString dbPath;
    dbPath = QFileDialog::getSaveFileName(this, "Save new database", "", "Hasher database files (*.hashdb)");

    // Create!
    HashDatabase *db = HashDatabase::createNew(dbPath.toStdString());

    // Immediately open the new database
    openDatabase(db);
}

void FileListWindow::openDatabase(HashDatabase *alreadyOpenDb)
{
    // TODO replace hashdb pointers with boost smart pointers
    // If passed a null pointer, just close everything
    if(alreadyOpenDb == NULL) {
        closeDatabase();
        return;
    }

    // If a database is already open, close it
    if(db != NULL && db != alreadyOpenDb) {
        delete db;
        db = NULL;
    }

    db = alreadyOpenDb;

    // TODO Enable UI and populate with current data
}

void FileListWindow::openDatabase(QString dbPath)
{
    HashDatabase *db = HashDatabase::open(dbPath.toStdString());
    openDatabase(db);
}

void FileListWindow::closeDatabase()
{
    // Close existing db
    if(db != NULL) {
        delete db;
        db = NULL;
    }

    // TODO Disable UI
}
