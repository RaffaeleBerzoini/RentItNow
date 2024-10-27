#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <sqlite3.h>
#include <string>

#ifdef DBMANAGER_EXPORT
#    define DB_EXPORT __declspec(dllexport)
#else
#    define DB_EXPORT __declspec(dllimport)
#endif


class DatabaseManager
{
public:
    DB_EXPORT DatabaseManager(const std::string& dbFilePath);

    // Set-up the Database if it does not exist
    DB_EXPORT void SetupDB();

private:
    std::string dbFilePath; // Database file path
    void CreateTables(sqlite3* db);
};

#endif // DATABASE_MANAGER_H