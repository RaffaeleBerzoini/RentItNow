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

std::optional<Interfaces::Car> BookingManager::GetBestCar(
    const std::vector<Interfaces::Car>& cars,
    const Interfaces::CarType& carType,
    const unsigned char& passengers)
{
    std::vector<Interfaces::Car> suitableCars;

    for (const auto& car : GetAvailableCars())
    {
        if (car.carSpecifics.type == carType && car.carSpecifics.seats >= passengers)
        {
            suitableCars.push_back(car);
        }
    }

    if (suitableCars.empty())
    {
        return std::nullopt;
    }

    // Get the car with lowest kms

}

BookingManager::BookingManager(std::shared_ptr<DatabaseManager> dbManagerPtr)
    : dbManager(dbManagerPtr)
{}