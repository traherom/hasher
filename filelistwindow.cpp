#include "filelistwindow.h"
#include "ui_filelistwindow.h"

#include "hashdatabase.h"
#include "sqlexception.h"

#include <iostream>
#include <list>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <QFileDialog>
#include <QString>

FileListWindow::FileListWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FileListWindow)
{
    ui->setupUi(this);
}

FileListWindow::~FileListWindow()
{
    delete ui;
}

void FileListWindow::updateUISidebar()
{
    // Ensure that sidebar reflects current DB state
    if(db) {
        // NSRL in database?
        if(db->hasNSRLData()) {
            ui->nsrlAddUpdateButton->setText(tr("Update NSRL data"));
        }
        else {
            ui->nsrlAddUpdateButton->setText(tr("Add NSRL data"));
        }

        ui->scanDateLabel->setText("");
        ui->scanFilesCountLabel->setText("");

        // Scan list
        ui->scansAvailableCombo->clear();
        std::list<shared_ptr<Scan>> scans = db->getAllScans();
        for(shared_ptr<Scan> scan : scans) {
            ui->scansAvailableCombo->addItem(scan->getName().c_str());
        }

        // All values updated, ready for user to interact
        ui->databaseTabs->setEnabled(true);
    }
    else {
        ui->databaseTabs->setEnabled(false);

        ui->nsrlAddUpdateButton->setText(tr("Add NSRL data"));

        ui->scanDateLabel->setText("");
        ui->scanFilesCountLabel->setText("");
        ui->scansAvailableCombo->clear();
        ui->scansAvailableCombo->addItem(tr("No database loaded"));
    }
}

void FileListWindow::createDatabasePrompt()
{
    // Open dialog to allow user to select where to save database
    QString dbPath;
    dbPath = QFileDialog::getSaveFileName(this, "Save new database", "", "Hasher database files (*.hashdb)");

    try {
        if(!dbPath.isEmpty()) {
            // Create!
            boost::shared_ptr<HashDatabase> db = HashDatabase::createNew(dbPath.toStdString());

            // Immediately open the new database
            openDatabase(db);
        }
    }
    catch(SQLException e) {
        std::cerr << e << std::endl;
    }
}

void FileListWindow::openDatabasePrompt()
{
    // Open dialog to allow user to select where to save database
    QString dbPath;
    dbPath = QFileDialog::getOpenFileName(this, "Select database", "", "Hasher database files (*.hashdb)");

    try {
        if(!dbPath.isEmpty()) {
            openDatabase(dbPath);
        }
    }
    catch(SQLException e) {
        std::cerr << e << std::endl;
    }
}

void FileListWindow::openDatabase(boost::shared_ptr<HashDatabase> alreadyOpenDb)
{
    // TODO replace hashdb pointers with boost smart pointers
    // If passed a null pointer, just close everything
    if(alreadyOpenDb == nullptr) {
        closeDatabase();
        return;
    }

    // Overwrite any existing database
    db = alreadyOpenDb;

    // Enable UI and populate with current data
    updateUISidebar();
}

void FileListWindow::openDatabase(QString dbPath)
{
    boost::shared_ptr<HashDatabase> db = HashDatabase::open(dbPath.toStdString());
    openDatabase(db);
}

void FileListWindow::closeDatabase()
{
    // Close existing db
    // TODO does this cause the pointer to lose the reference? (that's the goal)
    db.reset();

    // Disable UI
    updateUISidebar();
}

void FileListWindow::on_createButton_clicked()
{
    createDatabasePrompt();
}

void FileListWindow::on_openButton_clicked()
{
    openDatabasePrompt();
}

void FileListWindow::on_nsrlAddUpdateButton_clicked()
{
    // Check for newest version of NSRL database and compare to ours
    //
}

void FileListWindow::on_dbControlDock_visibilityChanged(bool visible)
{
    if(visible) {
        updateUISidebar();
    }
}

void FileListWindow::on_scanNowButton_clicked()
{
    // TODO open a scan wizard of some kind?
    try {
        shared_ptr<Scan> scan = db->addScan("test");
        ui->scansAvailableCombo->addItem(scan->getName().c_str());
        ui->scansAvailableCombo->setCurrentText(scan->getName().c_str());
    }
    catch(SQLException e) {
        std::cerr << e << std::endl;
    }
}
