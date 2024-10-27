#include <iostream>
#include "UserManager.h"

bool UserManager::AddUser(const Interfaces::User& user)
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

bool UserManager::UpdateUser(const std::string& driving_license, const Interfaces::User& user)
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

std::optional<Interfaces::User> UserManager::GetUser(const std::string& drivingLicense)
{
    return dbManager->GetUser(drivingLicense);
}

UserManager::UserManager(std::shared_ptr<DatabaseManager> dbManagerPtr)
    : dbManager(dbManagerPtr)
{}
