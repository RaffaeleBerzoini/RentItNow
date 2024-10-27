#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <sqlite3.h>
#include <string>
#include "StructureInterface.h"
#include <optional>

#ifdef DBMANAGER_EXPORT
#    define DB_EXPORT __declspec(dllexport)
#else
#    define DB_EXPORT __declspec(dllimport)
#endif


class DatabaseManager
{
public:
    DB_EXPORT DatabaseManager(const std::string& dbFilePath);

    DB_EXPORT bool AddUser(const User& user);
    DB_EXPORT bool UpdateUser(const std::string& driving_license, const User& user);
    DB_EXPORT bool RemoveUser(const std::string& drivingLicense);
    DB_EXPORT std::optional<User> GetUser(const std::string& drivingLicense);

private:
    std::string dbFilePath; // Database file path
    // Set-up the Database if it does not exist
    void SetupDB();
    void CreateTables(sqlite3* db);
    sqlite3* OpenDB();
};

#endif // DATABASE_MANAGER_H