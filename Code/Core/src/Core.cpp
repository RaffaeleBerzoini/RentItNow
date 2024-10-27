#include "Core.h"

namespace
{
std::string DATABASE_PATH = "database/database.db";
}

Core::Core()
    : databaseManager(std::make_shared<DatabaseManager>(DATABASE_PATH))
    , userManager(databaseManager)
{}

Core* Core::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new Core();
    }
    return instance;
}

UserManager& Core::GetUserManager()
{
    return userManager;
}
