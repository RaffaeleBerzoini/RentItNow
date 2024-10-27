#include <iostream>
#include "CarManager.h"

bool CarManager::AddCar(const Interfaces::Car& car)
{
    if (dbManager->AddCar(car))
    {
        std::cout << "Car " << car.licensePlate << " added" << std::endl;
        return true;
    }
    else
    {
        std::cout << "Car not added" << std::endl;
        return false;
    }
}

bool CarManager::UpdateCar(const std::string& licensePlate, const Interfaces::Car& car)
{
    if (dbManager->UpdateCar(licensePlate, car))
    {
        std::cout << "Car " << car.licensePlate << " updated" << std::endl;
        return true;
    }
    else
    {
        std::cout << "Car not updated" << std::endl;
        return false;
    }
}

bool CarManager::RemoveCar(const std::string& drivingLicense)
{
    if (dbManager->RemoveCar(drivingLicense))
    {
        std::cout << "Car " << drivingLicense << " removed" << std::endl;
        return true;
    }
    else
    {
        std::cout << "Car not removed" << std::endl;
        return false;
    }
}

std::optional<Interfaces::Car> CarManager::GetCar(const std::string& drivingLicense)
{
    return dbManager->GetCar(drivingLicense);
}

std::vector<Interfaces::Car> CarManager::GetAllCars()
{
    return dbManager->GetAllCars();
}

CarManager::CarManager(std::shared_ptr<DatabaseManager> dbManagerPtr)
    : dbManager(dbManagerPtr)
{}
