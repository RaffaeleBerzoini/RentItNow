#include <iostream>
#include "UserManager.h"

void UserManager::AddUser(const User& user)
{
    if (dbManager->AddUser(user))
    {
        std::cout << "User " << user.drivingLicense << " added" << std::endl;
    }
    else
    {
        std::cout << "User not added" << std::endl;
    }
}

void UserManager::UpdateUser(const std::string& driving_license, const User& user)
{
    if (dbManager->UpdateUser(driving_license, user))
    {
        std::cout << "User " << user.drivingLicense << " updated" << std::endl;
    }
    else
    {
        std::cout << "User not updated" << std::endl;
    }
}

void UserManager::RemoveUser(const std::string& drivingLicense)
{
    if (dbManager->RemoveUser(drivingLicense))
    {
        std::cout << "User " << drivingLicense << " removed" << std::endl;
    }
    else
    {
        std::cout << "User not removed" << std::endl;
    }
}

std::optional<User> UserManager::GetUser(const std::string& drivingLicense)
{
    return dbManager->GetUser(drivingLicense);
}

UserManager::UserManager(std::shared_ptr<DatabaseManager> dbManagerPtr)
    : dbManager(dbManagerPtr)
{}
