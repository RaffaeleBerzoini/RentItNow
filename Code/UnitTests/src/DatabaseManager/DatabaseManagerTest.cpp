#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_NO_PARALLEL
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
}