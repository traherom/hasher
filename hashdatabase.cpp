#include "hashdatabase.h"

#include <iostream>

HashDatabase::HashDatabase()
{
    // Create an in-memory database
    int err = sqlite3_open("", &db);
    if(err) {
        // TODO: unable to open
    }
}

HashDatabase::HashDatabase(string dbPath)
{
    // Only open existing databases
    int err = sqlite3_open_v2(dbPath.c_str(), &db, SQLITE_OPEN_READWRITE, NULL);
    if(err) {
        // TODO: unable to open
    }
}

HashDatabase::~HashDatabase()
{
    if(db != NULL) {
        sqlite3_close(db);
        db = NULL;
    }
}

HashDatabase *HashDatabase::createNew(string dbPath)
{
    // TODO: refuse to overwrite existing file?

    // Create db with schema
    sqlite3 *db = NULL;
    int err = sqlite3_open(dbPath.c_str(), &db);
    if(err) {
        // TODO throw exception
        return NULL;
    }

    sqlite3_exec(db,"CREATE TABLE NSRLProd ("
                        " ProductCode int unsigned NOT NULL,"
                        " ProductName varchar(1024),"
                        " ProductVersion varchar(1024),"
                        " OpSystemCode varchar(255),"
                        " MfgCode varchar(100),"
                        " Language varchar(1024),"
                     ")", NULL, NULL, NULL);

    /*CREATE TABLE IF NOT EXISTS NSRLFile (
            SHA1 char(41),
            MD5 char(32),
            CRC32 char(32),
            FileName varchar(1024),
            FileSize varchar(1024),
            ProductCode int unsigned,
            OpSystemCode varchar(255),
            Specialcode varchar(255),
            INDEX sha_index (SHA1)
        ) ENGINE=innodb DEFAULT CHARSET=utf8;
        CREATE TABLE IF NOT EXISTS systems (
            comp_id int unsigned NOT NULL AUTO_INCREMENT,
            name varchar(100) NOT NULL,
            PRIMARY KEY (comp_id),
            UNIQUE INDEX (name)
        ) ENGINE=innodb DEFAULT CHARSET=utf8;
        CREATE TABLE IF NOT EXISTS current_files (
            comp_id int unsigned NOT NULL,
            SHA1 char(41) NOT NULL,
            record_date timestamp NOT NULL DEFAULT NOW(),
            path varchar(1024) NOT NULL,
            path_hash char(32) NOT NULL,
            found bit DEFAULT false,
            FOREIGN KEY (comp_id) REFERENCES systems (comp_id) ON DELETE CASCADE,
            INDEX (comp_id, SHA1),
            INDEX (comp_id),
            UNIQUE INDEX (comp_id, path_hash)
        ) ENGINE=innodb DEFAULT CHARSET=utf8;
        CREATE TABLE IF NOT EXISTS reports (
            report_id int unsigned NOT NULL AUTO_INCREMENT,
            comp_id int unsigned NOT NULL,
            report_date timestamp NOT NULL DEFAULT NOW(),
            report mediumtext NOT NULL,
            PRIMARY KEY (report_id),
            FOREIGN KEY (comp_id) REFERENCES systems (comp_id) ON DELETE CASCADE
        ) ENGINE=innodb DEFAULT CHARSET=utf8;" | $SQL_CMD
        */



    // Done
    if(sqlite3_close(db)) {
        std::cerr << "Error closing during new database creation, continuing on" << std::endl;
    }

    // Return new database object already set up
    return open(dbPath);
}

HashDatabase *HashDatabase::open(string dbPath)
{
    HashDatabase *newDb = new HashDatabase(dbPath);
    return newDb;
}
