#ifndef HASHDATABASE_H
#define HASHDATABASE_H

#include <string>
using std::string;

#include <sqlite3.h>

class HashDatabase
{
public:
    ~HashDatabase();

    static HashDatabase *createNew(string dbPath);
    static HashDatabase *open(string dbPath);

private:
    HashDatabase();
    HashDatabase(string dbPath);

    sqlite3 *db;
};

#endif // HASHDATABASE_H
