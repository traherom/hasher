#include "hashdatabase.h"
#include "sqlexception.h"

#include <iostream>
#include <functional>

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

HashDatabase::HashDatabase()
    : HashDatabase("")
{
}

HashDatabase::HashDatabase(string dbPath)
    : mDbPath(dbPath)
{
    // Only open existing databases
    int err = sqlite3_open_v2(dbPath.c_str(), &mDb, SQLITE_OPEN_READWRITE, nullptr);
    if(err) {
        throw SQLException("Unable to open database", sqlite3_errmsg(mDb));
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

    // Create db with schema
    sqlite3 *db = nullptr;
    shared_ptr<sqlite3> dbProtector(db, std::ptr_fun(sqlite3_close));
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
        throw SQLException("Unable to create NSRL file table: ", sqlite3_errmsg(db));
    }

    sqlite3_exec(db, "CREATE TABLE scans ("
                            " id integer PRIMARY KEY,"
                            " name varchar(100) UNIQUE,"
                            " date datetime"
                        ")", nullptr, nullptr, nullptr);
    if(err) {
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
        throw SQLException("Unable to create files table: ", sqlite3_errmsg(db));
    }

    sqlite3_exec(db, "CREATE TABLE settings ("
                             "key char(10) PRIMARY KEY, "
                             "value varchar(255)"
                         ")", nullptr, nullptr, nullptr);
    if(err) {
        throw SQLException("Unable to create settings table: ", sqlite3_errmsg(db));
    }

    // Done
    if(sqlite3_close(db)) {
        std::cerr << "Error closing during new database creation, continuing on" << std::endl;
    }

    db = nullptr;

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

const string HashDatabase::getSetting(const string &key)
{
    string sql = "SELECT value FROM settings WHERE key=?";
    sqlite3_stmt *stmt = nullptr;

    if(sqlite3_prepare_v2(mDb, sql.c_str(), sql.length(), &stmt, NULL)) {
        throw SQLException("Unable to prepare to get setting: ", sqlite3_errmsg(mDb));
    }

    if(sqlite3_bind_text(stmt, 1, key.c_str(), key.length(), NULL)) {
        throw SQLException("Unable to bind key to get setting: ", sqlite3_errmsg(mDb));
    }

    string value = "";
    if(sqlite3_step(stmt) == SQLITE_ROW) {
        const char *cVal = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        if(cVal != nullptr) {
            value = cVal;
        }
    }

    sqlite3_finalize(stmt);

    return value;
}

const string HashDatabase::setSetting(const string &key, const string &value)
{
    string old = getSetting(key);
    if(old == value) {
        // No change
        return old;
    }

    string sql = "INSERT OR REPLACE INTO settings ('key', 'value') VALUES (?, ?)";
    sqlite3_stmt *stmt = nullptr;

    if(sqlite3_prepare_v2(mDb, sql.c_str(), sql.length(), &stmt, NULL)) {
        throw SQLException("Unable to prepare to set setting: ", sqlite3_errmsg(mDb));
    }

    if(sqlite3_bind_text(stmt, 1, key.c_str(), key.length(), NULL)) {
        throw SQLException("Unable to bind key to set setting: ", sqlite3_errmsg(mDb));
    }

    if(sqlite3_bind_text(stmt, 2, value.c_str(), value.length(), NULL)) {
        throw SQLException("Unable to bind value to set setting: ", sqlite3_errmsg(mDb));
    }

    if(sqlite3_step(stmt) != SQLITE_DONE) {
        throw SQLException("Unable to set setting: ", sqlite3_errmsg(mDb));
    }

    sqlite3_finalize(stmt);

    return old;
}

const string HashDatabase::getBasePath()
{
    return getSetting("base_path");
}

void HashDatabase::setBasePath(const string &path)
{
    setSetting("base_path", path);
}
