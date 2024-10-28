#include "User.h"

User::User(
    const std::string& drivingLicense,
    const std::string& name,
    const std::string& surname,
    const std::string& address,
    const std::string& creditCard)
    : user(name, surname, address, creditCard, drivingLicense)
{}

bool User::IsRegistered()
{
    return API::Common::GetUser(user.drivingLicense).has_value();
}

void User::RegisterToRentalService()
{
    if (!IsRegistered())
    {
        API::Common::AddUser(user);
        std::cout << "User registered successfully" << std::endl;
    }
    else
    {
        std::cout << "User already registered" << std::endl;
    }
}

void User::UpdateAddress(const std::string& address)
{
    if (!IsRegistered())
    {
        std::cout << "User is not registered" << std::endl;
        return;
    }
    user.address = address;
    API::Common::UpdateUser(user.drivingLicense, user);
}

void User::UpdateCreditCard(const std::string& creditCard)
{
    if (!IsRegistered())
    {
        std::cout << "User is not registered" << std::endl;
        return;
    }
    user.creditCard = creditCard;
    API::Common::UpdateUser(user.drivingLicense, user);
}

void User::UnregisterFromRentalService()
{
    if (!IsRegistered())
    {
        std::cout << "User is not registered" << std::endl;
        return;
    }
    API::Common::RemoveUser(user.drivingLicense);
}

void User::BookCar(
    const Interfaces::CarType& carType,
    const unsigned char& passengers,
    const Interfaces::CircleType& startCircle,
    const Interfaces::CircleType& endCircle,
    const int numOfDays)
{
    if (!IsRegistered())
    {
        std::cout << "User is not registered" << std::endl;
        return;
    }

    if (API::User::BookCar(user, carType, passengers, startCircle, endCircle, numOfDays))
    {
        std::cout << "Car booked successfully" << std::endl;
    }
    else
    {
        std::cout << "Car booking failed" << std::endl;
    }
}

void User::VisualizeTrips()
{
    if (!IsRegistered())
    {
        std::cout << "User is not registered" << std::endl;
        return;
    }

    auto trips = API::User::GetUserTrips(user.drivingLicense);
    if (trips.empty())
    {
        std::cout << "No trips found" << std::endl;
        return;
    }

    for (const auto& trip : trips)
    {
        std::cout << "-------------Trip-------------" << std::endl;
        std::cout << "Car type: " << trip.car.carTypeToString() << std::endl;
        std::cout << "License plate: " << trip.car.licensePlate << std::endl;
        std::cout << "Car brand: " << trip.car.brand << std::endl;
        std::cout << "Car model: " << trip.car.name << std::endl;
        std::cout << "Max Passengers: " << static_cast<int>(trip.car.carSpecifics.seats) << std::endl;
        std::cout << "Start circle: " << trip.trip.startCircleToString() << std::endl;
        std::cout << "End circle: " << trip.trip.destinationCircleToString() << std::endl;
        std::cout << "Start date: " << trip.trip.start_rental_date << std::endl;
        std::cout << "End date: " << trip.trip.end_rental_date << std::endl;
        std::cout << "Total cost: " << trip.trip.cost << std::endl;
        std::cout << std::endl;
    }
}
