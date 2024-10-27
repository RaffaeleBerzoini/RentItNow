#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <sqlite3.h>
#include <string>
#include "StructureInterface.h"
#include <optional>
#include <mutex>

#ifdef DBMANAGER_EXPORT
#    define DB_EXPORT __declspec(dllexport)
#else
#    define DB_EXPORT __declspec(dllimport)
#endif


class DatabaseManager
{
public:
    DB_EXPORT DatabaseManager(const std::string& dbFilePath);

    DB_EXPORT bool AddUser(const Interfaces::User& user);
    DB_EXPORT bool UpdateUser(const std::string& driving_license, const Interfaces::User& user);
    DB_EXPORT bool RemoveUser(const std::string& drivingLicense);
    DB_EXPORT std::optional<Interfaces::User> GetUser(const std::string& drivingLicense);

    DB_EXPORT bool AddCar(const Interfaces::Car& car);
    DB_EXPORT bool UpdateCar(const std::string& licensePlate, const Interfaces::Car& car);
    DB_EXPORT bool RemoveCar(const std::string& licensePlate);
    DB_EXPORT std::optional<Interfaces::Car> GetCar(const std::string& licensePlate);

private:
    std::mutex dbMutex;
    std::string dbFilePath; // Database file path
    // Set-up the Database if it does not exist
    void SetupDB();
    void CreateTables(sqlite3* db);
    sqlite3* OpenDB();
};

#endif // DATABASE_MANAGER_H