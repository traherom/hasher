#ifndef HASHDATABASE_H
#define HASHDATABASE_H

#include <string>
#include <list>
using std::string;

#include <sqlite3.h>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
using boost::shared_ptr;

#include "scan.h"
#include "scanfile.h"

class HashDatabase : public boost::enable_shared_from_this<HashDatabase>
{
public:
    HashDatabase(string dbPath);
    ~HashDatabase();

    static shared_ptr<HashDatabase> createNew(string dbPath);
    static shared_ptr<HashDatabase> open(string dbPath);

    bool hasNSRLData() { return false; }

    // Manage scans
    shared_ptr<Scan> addScan(string name);
    shared_ptr<Scan> getNewestScan();
    std::list<shared_ptr<Scan>> getAllScans() { return Scan::getAllScans(shared_from_this()); }

    inline sqlite3 *getDatabase() { return mDb; }

private:
    HashDatabase();

    sqlite3 *mDb;
};

#endif // HASHDATABASE_H
