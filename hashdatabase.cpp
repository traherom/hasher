#include "hashdatabase.h"
#include "sqlexception.h"

#include <iostream>

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

HashDatabase::HashDatabase()
{
    // Create an in-memory database
    int err = sqlite3_open("", &mDb);
    if(err) {
        // TODO: unable to open
        std::cerr << "Unable to open in-memory database: " << sqlite3_errmsg(mDb) << std::endl;
    }
}

HashDatabase::HashDatabase(string dbPath)
{
    // Only open existing databases
    int err = sqlite3_open_v2(dbPath.c_str(), &mDb, SQLITE_OPEN_READWRITE, nullptr);
    if(err) {
        // TODO: unable to open
        std::cerr << "Unable to open database: " << sqlite3_errmsg(mDb) << std::endl;
    }
}

HashDatabase::~HashDatabase()
{
    if(mDb != nullptr) {
        sqlite3_close(mDb);
        mDb = nullptr;
    }
}

shared_ptr<HashDatabase> HashDatabase::createNew(string dbPath)
{
    // TODO: refuse to overwrite existing file?
    // TODO ensure db is closed during exceptions

    // Create db with schema
    sqlite3 *db = nullptr;
    int err = sqlite3_open(dbPath.c_str(), &db);
    if(err) {
        throw SQLException("Unable to open database for creation", sqlite3_errmsg(db));
    }

    err = sqlite3_exec(db,"CREATE TABLE nsrl_prod ("
                            " ProductCode int unsigned NOT NULL,"
                            " ProductName varchar(1024),"
                            " ProductVersion varchar(1024),"
                            " OpSystemCode varchar(255),"
                            " MfgCode varchar(100),"
                            " Language varchar(1024)"
                          ")", nullptr, nullptr, nullptr);
    if(err) {
        sqlite3_close(db);
        throw SQLException("Unable to create NSRL product table: ", sqlite3_errmsg(db));
    }

    sqlite3_exec(db, "CREATE TABLE nsrl_file ("
                            " SHA1 char(41),"
                            " MD5 char(32),"
                            " CRC32 char(32),"
                            " FileName varchar(1024),"
                            " FileSize varchar(1024),"
                            " ProductCode int unsigned,"
                            " OpSystemCode varchar(255),"
                            " Specialcode varchar(255)"
                        ")", nullptr, nullptr, nullptr);
    if(err) {
        sqlite3_close(db);
        throw SQLException("Unable to create NSRL file table: ", sqlite3_errmsg(db));
    }

    sqlite3_exec(db, "CREATE TABLE scans ("
                            " id integer PRIMARY KEY,"
                            " name varchar(100) UNIQUE,"
                            " date datetime"
                        ")", nullptr, nullptr, nullptr);
    if(err) {
        sqlite3_close(db);
        throw SQLException("Unable to create scans table: ", sqlite3_errmsg(db));
    }

    sqlite3_exec(db, "CREATE TABLE files ("
                             "id integer PRIMARY KEY, "
                             "scan_id integer REFERENCES scans (id) ON DELETE CASCADE, "
                             "path text, "
                             "basename text, "
                             "md5 char(32), "
                             "sha1 char(32) "
                         ")", nullptr, nullptr, nullptr);
    if(err) {
        sqlite3_close(db);
        throw SQLException("Unable to create files table: ", sqlite3_errmsg(db));
    }

    sqlite3_exec(db, "CREATE TABLE settings ("
                             "key char(10), "
                             "value varchar(255)"
                         ")", nullptr, nullptr, nullptr);
    if(err) {
        sqlite3_close(db);
        throw SQLException("Unable to create settings table: ", sqlite3_errmsg(db));
    }

    // Done
    if(sqlite3_close(db)) {
        std::cerr << "Error closing during new database creation, continuing on" << std::endl;
    }

    // Return new database object already set up
    return open(dbPath);
}

shared_ptr<HashDatabase> HashDatabase::open(string dbPath)
{
    return boost::make_shared<HashDatabase>(dbPath);
}

shared_ptr<Scan> HashDatabase::addScan(string name)
{
    return Scan::addScan(shared_from_this(), name);
}
