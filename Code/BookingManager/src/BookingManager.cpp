#include "BookingManager.h"
#include <iostream>

#define KM_PER_HOP 5

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
    auto bestCar = suitableCars[0];
    int bestCarKms = dbManager->GetCarMilage(bestCar.licensePlate);
    for (const auto& car : suitableCars)
    {
        int carKms = dbManager->GetCarMilage(car.licensePlate);
        if (carKms < bestCarKms)
        {
            bestCar = car;
            bestCarKms = carKms;
        }
    }

    return bestCar;
}

int BookingManager::ComputeDistance(const Interfaces::CircleType& startCircle, const Interfaces::CircleType& endCircle)
{
    int start = static_cast<int>(startCircle);
    int end = static_cast<int>(endCircle);

    int distance = std::abs(end - start) + 1; // +1 because i have 1 hop to move inside the same circle
    distance *= KM_PER_HOP;

    return distance;
}

double BookingManager::ComputePrice(const Interfaces::CarSpecifics& carSpecifics, const int& distance)
{
    return carSpecifics.getPricePerKm() * distance;
}

bool BookingManager::BookCar(
    const Interfaces::User& user,
    const Interfaces::CarType& carType,
    const unsigned char& passengers,
    const Interfaces::CircleType& startCircle,
    const Interfaces::CircleType& endCircle,
    const int numOfDays)
{
    auto bestCar = GetBestCar(carType, passengers);
    if (!bestCar.has_value())
    {
        return false;
    }

    int user_id = dbManager->GetUserID(user.drivingLicense);
    int car_id = dbManager->GetCarID(bestCar->licensePlate);

    if (user_id == -1 || car_id == -1)
    {
        std::cout << "User or car not existent" << std::endl;
        return false;
    }

    int distance = ComputeDistance(startCircle, endCircle);
    double cost = ComputePrice(bestCar->carSpecifics, distance);

    std::string startDate = dbManager->GetCurrentDate();
    std::string endDate = dbManager->GetNextDate(numOfDays);

    Interfaces::Trip trip = {user_id, car_id, startCircle, endCircle, distance, cost, startDate, endDate};

    dbManager->AddTrip(trip);

    bestCar->status = Interfaces::CarStatus::RENTED;
    dbManager->UpdateCar(bestCar->licensePlate, *bestCar);

    return true;
}

BookingManager::BookingManager(std::shared_ptr<DatabaseManager> dbManagerPtr)
    : dbManager(dbManagerPtr)
{}