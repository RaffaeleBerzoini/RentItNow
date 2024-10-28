#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "Core.h"
#include <filesystem>

using CORE = Core* (*)();
CORE CorePtr = Core::GetInstance;

TEST_CASE("Business Logic")
{
    // There will be only one section in this test case since Core is a singleton
    // and removing the database file will affect all tests since Core creates the database
    // inside its constructor which is called only once.

    const std::string testDBPath = "database/test_db.db";
    Core::SetDatabasePath(testDBPath);

    // Remove any existing test database file to ensure a clean start
    if (std::filesystem::exists(testDBPath))
    {
        std::filesystem::remove(testDBPath);
    }

    SECTION("Core")
    {
        // Check if the Core instance is created
        REQUIRE(CorePtr() != nullptr);

        // Check if the database file is created
        REQUIRE(std::filesystem::exists(testDBPath));

        // Check if the Core instance is a singleton
        REQUIRE(CorePtr() == CorePtr());
    }
    
}