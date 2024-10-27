#include "DatabaseManager.h"
#include <iostream>
#include <filesystem>

DatabaseManager::DatabaseManager(const std::string& dbFilePath)
    : dbFilePath(dbFilePath)
{
    std::filesystem::path dbDirectory{};
    // Create a path object from the dbFilePath
    std::filesystem::path dbPath(dbFilePath);

    // Check if the directory path is empty
    if (!dbPath.has_parent_path() || dbPath.parent_path().empty())
    {
        throw std::runtime_error("Error: Database file path must include a directory.");
    }

    // Set dbDirectory to the parent directory
    dbDirectory = dbPath.parent_path();

    // Create the directory if it doesn't exist
    std::filesystem::create_directories(dbDirectory);
}

sqlite3* DatabaseManager::OpenDB()
{
    sqlite3* db;
    if (sqlite3_open(dbFilePath.c_str(), &db) != SQLITE_OK)
    {
        throw std::runtime_error("Error: Can't open database: " + std::string(sqlite3_errmsg(db)));
    }

    return db;
}

void DatabaseManager::SetupDB() {
    // Open the database
    sqlite3* db = OpenDB();

    CreateTables(db);

    // Close the database connection
    sqlite3_close(db);
}

void DatabaseManager::CreateTables(sqlite3* db) {
    const char* createCarsTable = R"(
        CREATE TABLE IF NOT EXISTS Cars (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            type TEXT NOT NULL CHECK (type IN ('ECO', 'MID-CLASS', 'DELUXE')),
            license_plate TEXT UNIQUE NOT NULL,
            brand TEXT NOT NULL,
            name TEXT NOT NULL,
            status TEXT NOT NULL CHECK (status IN ('available', 'rented', 'under_service'))
        );
    )";

    const char* createUsersTable = R"(
        CREATE TABLE IF NOT EXISTS Users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            surname TEXT NOT NULL,
            address TEXT NOT NULL,
            credit_card TEXT NOT NULL,
            driving_license TEXT UNIQUE NOT NULL
        );
    )";

    const char* createTripsTable = R"(
        CREATE TABLE IF NOT EXISTS Trips (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            car_id INTEGER NOT NULL,
            start_circle TEXT NOT NULL CHECK (start_circle IN ('Inner Circle', 'Middle Circle', 'Outer Circle')),
            destination_circle TEXT NOT NULL CHECK (destination_circle IN ('Inner Circle', 'Middle Circle', 'Outer Circle')),
            distance INTEGER NOT NULL,
            cost REAL NOT NULL,
            start_rental_date DATE NOT NULL,
            end_rental_date DATE NOT NULL,
            FOREIGN KEY (user_id) REFERENCES Users(id),
            FOREIGN KEY (car_id) REFERENCES Cars(id)
        );
    )";

    const char* createServicesTable = R"(
        CREATE TABLE IF NOT EXISTS Services (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            car_id INTEGER NOT NULL,
            service_date DATE NOT NULL,
            distance_since_last_service INTEGER NOT NULL,
            FOREIGN KEY (car_id) REFERENCES Cars(id)
        );
    )";

    const char* createCurrentDateTable = R"(
        CREATE TABLE IF NOT EXISTS CurrentDate (
            id INTEGER PRIMARY KEY DEFAULT 1,
            date DATE NOT NULL
        );
    )";

    char* errorMessage;

    // Create Cars table
    if (sqlite3_exec(db, createCarsTable, nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::cerr << "Error creating Cars table: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }

    // Create Users table
    if (sqlite3_exec(db, createUsersTable, nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::cerr << "Error creating Users table: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }

    // Create Trips table
    if (sqlite3_exec(db, createTripsTable, nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::cerr << "Error creating Trips table: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }

    // Create Services table
    if (sqlite3_exec(db, createServicesTable, nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::cerr << "Error creating Services table: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }

    // Create CurrentDate table
    if (sqlite3_exec(db, createCurrentDateTable, nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::cerr << "Error creating CurrentDate table: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }
}

