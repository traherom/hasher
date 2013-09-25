#ifndef SCAN_H
#define SCAN_H

#include <string>
#include <list>
using std::string;

#include <sqlite3.h>
#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

//#include "hashdatabase.h"
class HashDatabase;

class Scan
{
public:
    Scan(shared_ptr<HashDatabase> parentDb, sqlite3_int64 id);
    Scan(shared_ptr<HashDatabase> parentDb, string scanName);
    Scan(shared_ptr<HashDatabase> parentDb, sqlite3_int64 id, string name, sqlite3_int64 date);

    static shared_ptr<Scan> addScan(shared_ptr<HashDatabase> parentDb, string name);
    static shared_ptr<Scan> addAndPerformScan(shared_ptr<HashDatabase> parentDb, string name);

    static std::list<shared_ptr<Scan>> getAllScans(shared_ptr<HashDatabase> parentDb);

    void performScan();

    int getId() const { return mId; }
    const string &getName() const { return mName; }
    int getDate() const { return mDate; }

private:
    void populateCacheValues(sqlite3_stmt *stmt);

    // Helper that HashDatabase can friend to give us access to the sqlite database
    inline sqlite3 *db();

    shared_ptr<HashDatabase> mParentDb;

    sqlite3_int64 mId;
    string mName;
    sqlite3_int64 mDate;
};

#endif // SCAN_H
