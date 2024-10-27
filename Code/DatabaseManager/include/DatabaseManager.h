#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <sqlite3.h>
#include <string>
#include "StructureInterface.h"
#include <optional>
#include <mutex>
#include <vector>

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
    DB_EXPORT std::vector<Interfaces::User> GetAllUsers();
    DB_EXPORT int GetUserID(const std::string& drivingLicense);

    DB_EXPORT bool AddCar(const Interfaces::Car& car);
    DB_EXPORT bool UpdateCar(const std::string& licensePlate, const Interfaces::Car& car);
    DB_EXPORT bool RemoveCar(const std::string& licensePlate);
    DB_EXPORT std::optional<Interfaces::Car> GetCar(const std::string& licensePlate);
    DB_EXPORT std::vector<Interfaces::Car> GetAllCars();
    DB_EXPORT int GetCarID(const std::string& licensePlate);

    DB_EXPORT int GetCarMilage(const std::string& licensePlate);

    DB_EXPORT bool AddTrip(const Interfaces::Trip& trip);

    DB_EXPORT std::string GetCurrentDate();
    DB_EXPORT std::string GetNextDate(int numDays);
    DB_EXPORT bool NextDay();

    DB_EXPORT std::optional<Interfaces::Service> GetService(const std::string& licensePlate);

private:
    std::mutex dbMutex;
    std::string dbFilePath; // Database file path
    // Set-up the Database if it does not exist
    void SetupDB();
    void CreateTables(sqlite3* db);
    sqlite3* OpenDB();

    bool UpdateDatabase();

    bool AddService(const std::string& licensePlate);
};

#endif // DATABASE_MANAGER_H
