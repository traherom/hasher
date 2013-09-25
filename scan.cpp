#include <string>
using std::string;

#include <boost/make_shared.hpp>

#include "scan.h"
#include "hashdatabase.h"
#include "sqlexception.h"

Scan::Scan(shared_ptr<HashDatabase> parentDb, sqlite3_int64 id)
    : mParentDb(parentDb), mId(id)
{
    string sql = "SELECT id, name, date FROM scans WHERE id=?";

    sqlite3_stmt *stmt = nullptr;
    if(sqlite3_prepare_v2(db(), sql.c_str(), sql.length(), &stmt, nullptr)) {
        throw SQLException("Unable to pull scan from database via ID", sqlite3_errmsg(db()));
    }

    sqlite3_bind_int64(stmt, 1, id);
    sqlite3_step(stmt);
    populateCacheValues(stmt);
    sqlite3_finalize(stmt);
    stmt = nullptr;
}

Scan::Scan(shared_ptr<HashDatabase> parentDb, string scanName)
    : mParentDb(parentDb), mName(scanName)
{
    string sql = "SELECT id, name, date FROM scans WHERE name=?";

    sqlite3_stmt *stmt = nullptr;
    if(sqlite3_prepare_v2(db(), sql.c_str(), sql.length(), &stmt, nullptr)) {
        throw SQLException("Unable to pull scan from database via name", sqlite3_errmsg(db()));
    }

    sqlite3_bind_text(stmt, 1, scanName.c_str(), scanName.length(), SQLITE_STATIC);
    sqlite3_step(stmt);
    populateCacheValues(stmt);
    sqlite3_finalize(stmt);
    stmt = nullptr;
}

Scan::Scan(shared_ptr<HashDatabase> parentDb, sqlite3_int64 id, string name, sqlite3_int64 date)
    : mParentDb(parentDb), mId(id), mName(name), mDate(date)
{

}

sqlite3 *Scan::db()
{
    return mParentDb->getDatabase();
}

shared_ptr<Scan> Scan::addScan(shared_ptr<HashDatabase> parentDb, string name)
{
    string sql = "INSERT INTO scans (name, date) VALUES (?, CURRENT_TIMESTAMP)";

    sqlite3 *db = parentDb->getDatabase();

    sqlite3_stmt *stmt = nullptr;
    if(sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &stmt, NULL)) {
        throw SQLException("Unable to prepare add scan statement", sqlite3_errmsg(db));
    }

    if(sqlite3_bind_text(stmt, 1, name.c_str(), name.length(), NULL)) {
        throw SQLException("Unable to bind add scan statement", sqlite3_errmsg(db));
    }

    if(sqlite3_step(stmt) != SQLITE_DONE) {
        throw SQLException("Failed to add scan", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    stmt = nullptr;

    return boost::make_shared<Scan>(parentDb, sqlite3_last_insert_rowid(db));
}

shared_ptr<Scan> Scan::addAndPerformScan(shared_ptr<HashDatabase> parentDb, string name)
{
    shared_ptr<Scan> scan = addScan(parentDb, name);
    scan->performScan();
    return scan;
}

std::list<shared_ptr<Scan>> Scan::getAllScans(shared_ptr<HashDatabase> parentDb)
{
    std::list<shared_ptr<Scan>> scans;

    string sql = "SELECT id, name, date FROM scans";
    sqlite3 *db = parentDb->getDatabase();
    sqlite3_stmt *stmt = nullptr;

    if(sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &stmt, NULL)) {
        throw SQLException("Unable to retrieve scans from database", sqlite3_errmsg(db));
    }

    while(sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 1);
        string name(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        int date = sqlite3_column_int(stmt, 3);

        std::cout << id << " " << name << " " << date << std::endl;
        scans.push_back(boost::make_shared<Scan>(parentDb, id, name, date));
    }

    sqlite3_finalize(stmt);
    stmt = nullptr;


    return scans;
}

void Scan::populateCacheValues(sqlite3_stmt *stmt)
{
    mId = sqlite3_column_int(stmt, 1);
    mName = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
    mDate = sqlite3_column_int(stmt, 3);
}

void Scan::performScan()
{
    auto db = mParentDb->getDatabase();

}
