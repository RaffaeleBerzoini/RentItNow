#include <iostream>
#include "Core.h"

Core::Core()
{
    databaseManager = std::make_shared<DatabaseManager>(DATABASE_PATH);
    userManager = std::make_unique<UserManager>(UserManager(databaseManager));
    carManager = std::make_unique<CarManager>(CarManager(databaseManager));
    bookingManager = std::make_unique<BookingManager>(BookingManager(databaseManager));    
}

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
    return *userManager;
}

CarManager& Core::GetCarManager()
{
    return *carManager;
}

BookingManager& Core::GetBookingManager()
{
    return *bookingManager;
}

void Core::NextDay()
{
    if (databaseManager->NextDay())
    {
        std::cout << "Next day : " << databaseManager->GetCurrentDate() << std::endl;
    }
    else
    {
        throw std::runtime_error("Next day failed");
    }
}
