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
    dbManager.SetupDB();

    SECTION("Database file is created")
    {
        REQUIRE(std::filesystem::exists(testDBPath));
    }

    // Open the database for further inspection
    sqlite3* db;
    int rc = sqlite3_open(testDBPath.c_str(), &db);
    REQUIRE(rc == SQLITE_OK);

    SECTION("Cars table exists")
    {
        sqlite3_stmt* stmt;
        const char* query = "SELECT name FROM sqlite_master WHERE type='table' AND name='Cars';";
        rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
        REQUIRE(rc == SQLITE_OK);
        REQUIRE(sqlite3_step(stmt) == SQLITE_ROW);
        sqlite3_finalize(stmt);
    }

    SECTION("Users table exists")
    {
        sqlite3_stmt* stmt;
        const char* query = "SELECT name FROM sqlite_master WHERE type='table' AND name='Users';";
        rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
        REQUIRE(rc == SQLITE_OK);
        REQUIRE(sqlite3_step(stmt) == SQLITE_ROW);
        sqlite3_finalize(stmt);
    }

    SECTION("Trips table exists")
    {
        sqlite3_stmt* stmt;
        const char* query = "SELECT name FROM sqlite_master WHERE type='table' AND name='Trips';";
        rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
        REQUIRE(rc == SQLITE_OK);
        REQUIRE(sqlite3_step(stmt) == SQLITE_ROW);
        sqlite3_finalize(stmt);
    }

    SECTION("Services table exists")
    {
        sqlite3_stmt* stmt;
        const char* query = "SELECT name FROM sqlite_master WHERE type='table' AND name='Services';";
        rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
        REQUIRE(rc == SQLITE_OK);
        REQUIRE(sqlite3_step(stmt) == SQLITE_ROW);
        sqlite3_finalize(stmt);
    }

    SECTION("CurrentDate table exists")
    {
        sqlite3_stmt* stmt;
        const char* query = "SELECT name FROM sqlite_master WHERE type='table' AND name='CurrentDate';";
        rc = sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
        REQUIRE(rc == SQLITE_OK);
        REQUIRE(sqlite3_step(stmt) == SQLITE_ROW);
        sqlite3_finalize(stmt);
    }

    // Clean up the database connection
    sqlite3_close(db);

    // Remove test database file after tests
    std::filesystem::remove(testDBPath);
}
