#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "DatabaseManager.h"
#include <sqlite3.h>
#include <filesystem>
#include <iostream>

TEST_CASE("DatabaseManager initializes the database file and tables", "[DatabaseManager]")
{
    const std::string testDBPath = "database/test_db.db";
    std::cout << "Test database path: " << testDBPath << std::endl;

    // Remove any existing test database file to ensure a clean start
    if (std::filesystem::exists(testDBPath))
    {
        std::filesystem::remove(testDBPath);
    }

    // Instantiate DatabaseManager and set up the database
    DatabaseManager dbManager(testDBPath);
    
    SECTION("DatabaseManager is initialized with errors")
    {
        REQUIRE_THROWS(DatabaseManager("test_db.db"));
    }

    SECTION("Database file is created")
    {
        REQUIRE(std::filesystem::exists(testDBPath));
    }

    SECTION("Cars table exists")
    {
        // Open the database for further inspection
        sqlite3* db;
        int rc = sqlite3_open(testDBPath.c_str(), &db);
        REQUIRE(rc == SQLITE_OK);
        sqlite3_stmt* stmt;
        const char* query = "SELECT name FROM sqlite_master WHERE type='table' AND name='Cars';";
        rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
        REQUIRE(rc == SQLITE_OK);
        REQUIRE(sqlite3_step(stmt) == SQLITE_ROW);
        sqlite3_finalize(stmt);
        // Clean up the database connection
        sqlite3_close(db);
    }

    SECTION("Users table exists")
    {
        // Open the database for further inspection
        sqlite3* db;
        int rc = sqlite3_open(testDBPath.c_str(), &db);
        REQUIRE(rc == SQLITE_OK);
        sqlite3_stmt* stmt;
        const char* query = "SELECT name FROM sqlite_master WHERE type='table' AND name='Users';";
        rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
        REQUIRE(rc == SQLITE_OK);
        REQUIRE(sqlite3_step(stmt) == SQLITE_ROW);
        sqlite3_finalize(stmt);
        // Clean up the database connection
        sqlite3_close(db);
    }

    SECTION("Trips table exists")
    {
        // Open the database for further inspection
        sqlite3* db;
        int rc = sqlite3_open(testDBPath.c_str(), &db);
        REQUIRE(rc == SQLITE_OK);
        sqlite3_stmt* stmt;
        const char* query = "SELECT name FROM sqlite_master WHERE type='table' AND name='Trips';";
        rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
        REQUIRE(rc == SQLITE_OK);
        REQUIRE(sqlite3_step(stmt) == SQLITE_ROW);
        sqlite3_finalize(stmt);
        // Clean up the database connection
        sqlite3_close(db);
    }

    SECTION("Services table exists")
    {
        // Open the database for further inspection
        sqlite3* db;
        int rc = sqlite3_open(testDBPath.c_str(), &db);
        REQUIRE(rc == SQLITE_OK);
        sqlite3_stmt* stmt;
        const char* query = "SELECT name FROM sqlite_master WHERE type='table' AND name='Services';";
        rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
        REQUIRE(rc == SQLITE_OK);
        REQUIRE(sqlite3_step(stmt) == SQLITE_ROW);
        sqlite3_finalize(stmt);
        // Clean up the database connection
        sqlite3_close(db);
    }

    SECTION("CurrentDate table exists")
    {
        // Open the database for further inspection
        sqlite3* db;
        int rc = sqlite3_open(testDBPath.c_str(), &db);
        REQUIRE(rc == SQLITE_OK);
        sqlite3_stmt* stmt;
        const char* query = "SELECT name FROM sqlite_master WHERE type='table' AND name='CurrentDate';";
        rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
        REQUIRE(rc == SQLITE_OK);
        REQUIRE(sqlite3_step(stmt) == SQLITE_ROW);
        sqlite3_finalize(stmt);
        // Clean up the database connection
        sqlite3_close(db);
    }
}

TEST_CASE("User management")
{
    const std::string testDBPath = "database/test_db.db";
    std::cout << "Test database path: " << testDBPath << std::endl;

    // Remove any existing test database file to ensure a clean start
    if (std::filesystem::exists(testDBPath))
    {
        std::filesystem::remove(testDBPath);
    }

    // Instantiate DatabaseManager and set up the database
    DatabaseManager dbManager(testDBPath);

    SECTION("Add and get existing user")
    {
        User user1("John", "Doe", "123 Main St", "1234 5678 9012 3456", "AA0011");
        REQUIRE(dbManager.AddUser(user1));

        User user2 = dbManager.GetUser("AA0011").value();
        REQUIRE(user1 == user2);

        User user3("Jane", "Doe", "456 Elm St", "9876 5432 1098 7654", "BB0022");
        REQUIRE(dbManager.AddUser(user3));

        User user4 = dbManager.GetUser("BB0022").value();
        REQUIRE(user3 == user4);

        REQUIRE(user2 != user4);
    }

    SECTION("Add and remove user")
    {
        User user1("John", "Doe", "123 Main St", "1234 5678 9012 3456", "AA0011");
        REQUIRE(dbManager.AddUser(user1));

        User user2 = dbManager.GetUser("AA0011").value();
        REQUIRE(user1 == user2);

        REQUIRE(dbManager.RemoveUser("AA0011"));
        REQUIRE_FALSE(dbManager.GetUser("AA0011").has_value());
    }

    SECTION("Add and update user")
    {
        User user1("John", "Doe", "123 Main St", "1234 5678 9012 3456", "AA0011");
        REQUIRE(dbManager.AddUser(user1));

        User user2 = dbManager.GetUser("AA0011").value();
        REQUIRE(user1 == user2);

        User user3("Jane", "Doe", "456 Elm St", "9876 5432 1098 7654", "BB0022");
        REQUIRE(dbManager.UpdateUser("AA0011", user3));

        // The driving license should not be updated
        REQUIRE_FALSE(dbManager.GetUser("BB0022").has_value());

        User user4 = dbManager.GetUser("AA0011").value();
        REQUIRE(user3.address == user4.address);
        REQUIRE(user3.creditCard == user4.creditCard);
        REQUIRE(user3.name == user4.name);
        REQUIRE(user3.surname == user4.surname);
        REQUIRE_FALSE(user3.drivingLicense == user4.drivingLicense);
    }

    SECTION("Multiple operations")
    {
        // Attempt to get a non-existent user
        REQUIRE_FALSE(dbManager.GetUser("NON_EXISTENT").has_value());

        // Add some users
        User user1("John", "Doe", "123 Main St", "1234 5678 9012 3456", "AA0011");
        User user2("Jane", "Doe", "456 Elm St", "9876 5432 1098 7654", "BB0022");
        User user3("Jim", "Beam", "789 Pine St", "5555 4444 3333 2222", "CC0033");

        REQUIRE(dbManager.AddUser(user1));
        REQUIRE(dbManager.AddUser(user2));
        REQUIRE(dbManager.AddUser(user3));

        // Verify that users can be retrieved correctly
        REQUIRE(dbManager.GetUser("AA0011").has_value());
        REQUIRE(dbManager.GetUser("BB0022").has_value());
        REQUIRE(dbManager.GetUser("CC0033").has_value());

        // Remove a user
        REQUIRE(dbManager.RemoveUser("BB0022"));
        REQUIRE_FALSE(dbManager.GetUser("BB0022").has_value());

        // Update user1's information and verify
        User updatedUser1("John", "Doe", "321 New St", "1234 5678 9012 3456", "AA0011");
        REQUIRE(dbManager.UpdateUser("AA0011", updatedUser1));

        // Check if the updated information is correct
        User fetchedUser1 = dbManager.GetUser("AA0011").value();
        REQUIRE(updatedUser1.address == fetchedUser1.address);
        REQUIRE(updatedUser1.creditCard == fetchedUser1.creditCard);
        REQUIRE(updatedUser1.name == fetchedUser1.name);
        REQUIRE(updatedUser1.surname == fetchedUser1.surname);
        REQUIRE(updatedUser1.drivingLicense == fetchedUser1.drivingLicense);

        // Verify that user3 exists before removing
        REQUIRE(dbManager.GetUser("CC0033").has_value());

        // Remove user3
        REQUIRE(dbManager.RemoveUser("CC0033"));
        REQUIRE_FALSE(dbManager.GetUser("CC0033").has_value());
    }
}

TEST_CASE("Car management")
{
    const std::string testDBPath = "database/test_db.db";
    std::cout << "Test database path: " << testDBPath << std::endl;

    // Remove any existing test database file to ensure a clean start
    if (std::filesystem::exists(testDBPath))
    {
        std::filesystem::remove(testDBPath);
    }

    // Instantiate DatabaseManager and set up the database
    DatabaseManager dbManager(testDBPath);

    SECTION("Add and get existing car")
    {
        Car car1(CarType::ECO, "ABC123", "Toyota", "Corolla", CarStatus::AVAILABLE);
        REQUIRE(dbManager.AddCar(car1));

        Car car2 = dbManager.GetCar("ABC123").value();
        REQUIRE(car1 == car2);

        Car car3(CarType::MID_CLASS, "DEF456", "Honda", "Civic", CarStatus::RENTED);
        REQUIRE(dbManager.AddCar(car3));

        Car car4 = dbManager.GetCar("DEF456").value();
        REQUIRE(car3 == car4);

        REQUIRE(car2 != car4);

        Car car5(CarType::DELUXE, "GHI789", "BMW", "M5", CarStatus::UNDER_SERVICE);
        REQUIRE(dbManager.AddCar(car5));

        Car car6 = dbManager.GetCar("GHI789").value();
        REQUIRE(car5 == car6);

        REQUIRE(car4 != car6);
    }

    SECTION("Add and remove car")
    {
        Car car1(CarType::ECO, "ABC123", "Toyota", "Corolla", CarStatus::AVAILABLE);
        REQUIRE(dbManager.AddCar(car1));

        Car car2 = dbManager.GetCar("ABC123").value();
        REQUIRE(car1 == car2);

        REQUIRE(dbManager.RemoveCar("ABC123"));
        REQUIRE_FALSE(dbManager.GetCar("ABC123").has_value());
    }

    SECTION("Add and update car")
    {
        Car car1(CarType::ECO, "ABC123", "Toyota", "Corolla", CarStatus::AVAILABLE);
        REQUIRE(dbManager.AddCar(car1));

        Car car2 = dbManager.GetCar("ABC123").value();
        REQUIRE(car1 == car2);

        Car car3(CarType::MID_CLASS, "DEF456", "Honda", "Civic", CarStatus::RENTED);
        REQUIRE(dbManager.UpdateCar("ABC123", car3));

        // The license plate should not be updated
        REQUIRE_FALSE(dbManager.GetCar("DEF456").has_value());

        Car car4 = dbManager.GetCar("ABC123").value();
        REQUIRE(car3.brand == car4.brand);
        REQUIRE(car3.name == car4.name);
        REQUIRE(car3.carSpecifics.type == car4.carSpecifics.type);
        // licensePlate is not updated
        REQUIRE_FALSE(car3.licensePlate == car4.licensePlate);
        REQUIRE(car3.status == car4.status);
    }

    SECTION("Multiple operations")
    {
        // Attempt to get a non-existent car
        REQUIRE_FALSE(dbManager.GetCar("NON_EXISTENT").has_value());

        // Add some cars
        Car car1(CarType::ECO, "ABC123", "Toyota", "Corolla", CarStatus::AVAILABLE);
        Car car2(CarType::MID_CLASS, "DEF456", "Honda", "Civic", CarStatus::RENTED);
        Car car3(CarType::DELUXE, "GHI789", "BMW", "M5", CarStatus::UNDER_SERVICE);

        REQUIRE(dbManager.AddCar(car1));
        REQUIRE(dbManager.AddCar(car2));
        REQUIRE(dbManager.AddCar(car3));

        // Verify that cars can be retrieved correctly
        REQUIRE(dbManager.GetCar("ABC123").has_value());
        REQUIRE(dbManager.GetCar("DEF456").has_value());
        REQUIRE(dbManager.GetCar("GHI789").has_value());

        // Remove a car
        REQUIRE(dbManager.RemoveCar("DEF456"));
        REQUIRE_FALSE(dbManager.GetCar("DEF456").has_value());

        // Update car1's information and verify
        Car updatedCar1(CarType::ECO, "ABC123", "Toyota", "Corolla", CarStatus::RENTED);
        REQUIRE(dbManager.UpdateCar("ABC123", updatedCar1));

        // Check if the updated information is correct
        Car fetchedCar1 = dbManager.GetCar("ABC123").value();
        REQUIRE(updatedCar1.brand == fetchedCar1.brand);
        REQUIRE(updatedCar1.name == fetchedCar1.name);
        REQUIRE(updatedCar1.carSpecifics.type == fetchedCar1.carSpecifics.type);
        REQUIRE(updatedCar1.licensePlate == fetchedCar1.licensePlate);
        REQUIRE(updatedCar1.status == fetchedCar1.status);

        // Verify that car3 exists before removing
        REQUIRE(dbManager.GetCar("GHI789").has_value());

        // Remove car3
        REQUIRE(dbManager.RemoveCar("GHI789"));
        REQUIRE_FALSE(dbManager.GetCar("GHI789").has_value());
    }
}