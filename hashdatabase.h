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

    // Misc
    const string &getDatabasePath() const { return mDbPath; }

    const string getSetting(const string &key);
    const string setSetting(const string &key, const string &value);

    const string getBasePath();
    void setBasePath(const string &path);

    // NSRL
    bool hasNSRLData() const { return false; }

    // Manage scans
    shared_ptr<Scan> addScan(string name);
    shared_ptr<Scan> getNewestScan();
    std::list<shared_ptr<Scan>> getAllScans() { return Scan::getAllScans(shared_from_this()); }

    inline sqlite3 *getDatabase() { return mDb; }

private:
    HashDatabase();

    string mDbPath;
    sqlite3 *mDb;
};

#endif // HASHDATABASE_H
