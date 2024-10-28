#include "Core.h"
#include "Interface.h"

using namespace API;

using CORE = Core* (*)();
CORE CorePtr = Core::GetInstance;

bool Common::AddUser(const Interfaces::User& user)
{
    return CorePtr()->GetUserManager().AddUser(user);
}

bool Common::UpdateUser(const std::string& driving_license, const Interfaces::User& user)
{
    return CorePtr()->GetUserManager().UpdateUser(driving_license, user);
}

bool Common::RemoveUser(const std::string& drivingLicense)
{
    return CorePtr()->GetUserManager().RemoveUser(drivingLicense);
}

std::optional<Interfaces::User> Common::GetUser(const std::string& drivingLicense)
{
    return CorePtr()->GetUserManager().GetUser(drivingLicense);
}

std::vector<Interfaces::User> Boss::GetAllUsers()
{
    return CorePtr()->GetUserManager().GetAllUsers();
}

bool Boss::AddCar(const Interfaces::Car& car)
{
    return CorePtr()->GetCarManager().AddCar(car);
}

bool Boss::UpdateCar(const std::string& licensePlate, const Interfaces::Car& car)
{
    return CorePtr()->GetCarManager().UpdateCar(licensePlate, car);
}

bool Boss::RemoveCar(const std::string& licensePlate)
{
    return CorePtr()->GetCarManager().RemoveCar(licensePlate);
}

std::optional<Interfaces::Car> Boss::GetCar(const std::string& licensePlate)
{
    return CorePtr()->GetCarManager().GetCar(licensePlate);
}

std::vector<Interfaces::Car> Boss::GetAllCars()
{
    return CorePtr()->GetCarManager().GetAllCars();
}

bool API::User::BookCar(
    const Interfaces::User& user,
    const Interfaces::CarType& carType,
    const unsigned char& passengers,
    const Interfaces::CircleType& startCircle,
    const Interfaces::CircleType& endCircle,
    const int numOfDays)
{
    return CorePtr()->GetBookingManager().BookCar(user, carType, passengers, startCircle, endCircle, numOfDays);
}

std::vector<Interfaces::TripInfo> API::User::GetUserTrips(const std::string& drivingLicense)
{
    return CorePtr()->GetBookingManager().GetUserTrips(drivingLicense);
}

void API::Server::NextDay()
{
    CorePtr()->NextDay();
}

std::string API::Server::GetCurrentDate()
{
    return CorePtr()->GetCurrentDate();
}
