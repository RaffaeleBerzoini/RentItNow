#include "BookingManager.h"

std::vector<Interfaces::Car> BookingManager::GetAvailableCars()
{
    std::vector<Interfaces::Car> allCars = dbManager->GetAllCars();
    std::vector<Interfaces::Car> availableCars;

    for (const auto& car : allCars)
    {
        if (car.status == Interfaces::CarStatus::AVAILABLE)
        {
            availableCars.push_back(car);
        }
    }

    return availableCars;
}

BookingManager::BookingManager(std::shared_ptr<DatabaseManager> dbManagerPtr)
    : dbManager(dbManagerPtr)
{}