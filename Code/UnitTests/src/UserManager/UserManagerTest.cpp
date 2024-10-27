#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "UserManager.h"
#include <filesystem>

/*
#include <iostream>
#include "UserManager.h"

bool UserManager::AddUser(const User& user)
{
    if (dbManager->AddUser(user))
    {
        std::cout << "User " << user.drivingLicense << " added" << std::endl;
        return true;
    }
    else
    {
        std::cout << "User not added" << std::endl;
        return false;
    }
}

bool UserManager::UpdateUser(const std::string& driving_license, const User& user)
{
    if (dbManager->UpdateUser(driving_license, user))
    {
        std::cout << "User " << user.drivingLicense << " updated" << std::endl;
        return true;
    }
    else
    {
        std::cout << "User not updated" << std::endl;
        return false;
    }
}

bool UserManager::RemoveUser(const std::string& drivingLicense)
{
    if (dbManager->RemoveUser(drivingLicense))
    {
        std::cout << "User " << drivingLicense << " removed" << std::endl;
        return true;
    }
    else
    {
        std::cout << "User not removed" << std::endl;
        return false;
    }
}

std::optional<User> UserManager::GetUser(const std::string& drivingLicense)
{
    return dbManager->GetUser(drivingLicense);
}

UserManager::UserManager(std::shared_ptr<DatabaseManager> dbManagerPtr)
    : dbManager(dbManagerPtr)
{}

*/

TEST_CASE("UserManagerTest")
{
    const std::string testDBPath = "database/test_db.db";

    // Remove any existing test database file to ensure a clean start
    if (std::filesystem::exists(testDBPath))
    {
        std::filesystem::remove(testDBPath);
    }

    auto dbManager = std::make_shared<DatabaseManager>(testDBPath);
    UserManager userManager(dbManager);

    SECTION("AddUser")
    {
        User user("John", "Doe", "123 Main St", "1234 5678 9012 3456", "AA0011");
        REQUIRE(userManager.AddUser(user));

        auto userFromDB = dbManager->GetUser("AA0011");
        REQUIRE(userFromDB.has_value());
        REQUIRE(userFromDB.value() == user);
    }

    SECTION("UpdateUser")
    {
        User user("John", "Doe", "123 Main St", "1234 5678 9012 3456", "AA0011");
        REQUIRE(userManager.AddUser(user));

        User updatedUser("Jane", "Smith", "456 Elm St", "1234 5678 9012 3456", "AA0011");
        REQUIRE(userManager.UpdateUser("AA0011", updatedUser));

        auto userFromDB = dbManager->GetUser("AA0011");
        REQUIRE(userFromDB.has_value());
        REQUIRE(userFromDB.value() == updatedUser);
    }

    SECTION("RemoveUser")
    {
        User user("John", "Doe", "123 Main St", "1234 5678 9012 3456", "AA0011");
        REQUIRE(userManager.AddUser(user));

        REQUIRE(userManager.RemoveUser("AA0011"));

        auto userFromDB = dbManager->GetUser("AA0011");
        REQUIRE(!userFromDB.has_value());
    }

    SECTION("GetUser")
    {
        User user("John", "Doe", "123 Main St", "1234 5678 9012 3456", "AA0011");
        REQUIRE(userManager.AddUser(user));

        auto userFromDB = userManager.GetUser("AA0011");
        REQUIRE(userFromDB.has_value());
        REQUIRE(userFromDB.value() == user);
    }

        SECTION("Multiple operations")
    {
        // Adding initial users
        User user1("John", "Doe", "123 Main St", "1234 5678 9012 3456", "AA0011");
        User user2("Alice", "Johnson", "456 Oak St", "2345 6789 0123 4567", "BB0022");
        REQUIRE(userManager.AddUser(user1));
        REQUIRE(userManager.AddUser(user2));

        // Verifying users have been added
        auto userFromDB1 = userManager.GetUser("AA0011");
        REQUIRE(userFromDB1.has_value());
        REQUIRE(userFromDB1.value() == user1);

        auto userFromDB2 = userManager.GetUser("BB0022");
        REQUIRE(userFromDB2.has_value());
        REQUIRE(userFromDB2.value() == user2);

        // Updating user1
        User updatedUser1("John", "Smith", "789 Maple St", "1234 5678 9012 3456", "AA0011");
        REQUIRE(userManager.UpdateUser("AA0011", updatedUser1));

        // Verifying update
        userFromDB1 = userManager.GetUser("AA0011");
        REQUIRE(userFromDB1.has_value());
        REQUIRE(userFromDB1.value() == updatedUser1);

        // Removing user2
        REQUIRE(userManager.RemoveUser("BB0022"));

        // Verifying removal
        userFromDB2 = userManager.GetUser("BB0022");
        REQUIRE(!userFromDB2.has_value());

        // Adding a new user with the same ID as a removed user
        User user3("Bob", "Brown", "321 Cedar St", "3456 7890 1234 5678", "BB0022");
        REQUIRE(userManager.AddUser(user3));

        // Verifying the new user with the reused ID
        auto userFromDB3 = userManager.GetUser("BB0022");
        REQUIRE(userFromDB3.has_value());
        REQUIRE(userFromDB3.value() == user3);

        // Checking final state in the database
        userFromDB1 = userManager.GetUser("AA0011");
        REQUIRE(userFromDB1.has_value());
        REQUIRE(userFromDB1.value() == updatedUser1);

        userFromDB2 = userManager.GetUser("BB0022");
        REQUIRE(userFromDB2.has_value());
        REQUIRE(userFromDB2.value() == user3);
    }
}