#include "Core.h"
#include "Interface.h"
#include "CoutSilencer.h"

using namespace API;

static CoutSilencer silencer;

using CORE = Core* (*)();
CORE CorePtr = Core::GetInstance;

bool Common::AddUser(const Interfaces::User& user)
{
    silencer.SilenceCout();
    bool result = CorePtr()->GetUserManager().AddUser(user);
    silencer.RestoreCout();
    return result;
}

bool Common::UpdateUser(const std::string& driving_license, const Interfaces::User& user)
{
    silencer.SilenceCout();
    bool result = CorePtr()->GetUserManager().UpdateUser(driving_license, user);
    silencer.RestoreCout();
    return result;
}

bool Common::RemoveUser(const std::string& drivingLicense)
{
    silencer.SilenceCout();
    bool result = CorePtr()->GetUserManager().RemoveUser(drivingLicense);
    silencer.RestoreCout();
    return result;
}

std::optional<Interfaces::User> Common::GetUser(const std::string& drivingLicense)
{
    silencer.SilenceCout();
    auto result = CorePtr()->GetUserManager().GetUser(drivingLicense);
    silencer.RestoreCout();
    return result;
}

std::vector<Interfaces::User> Boss::GetAllUsers()
{
    silencer.SilenceCout();
    auto result = CorePtr()->GetUserManager().GetAllUsers();
    silencer.RestoreCout();
    return result;
}

bool Boss::AddCar(const Interfaces::Car& car)
{
    silencer.SilenceCout();
    bool result = CorePtr()->GetCarManager().AddCar(car);
    silencer.RestoreCout();
    return result;
}

bool Boss::UpdateCar(const std::string& licensePlate, const Interfaces::Car& car)
{
    silencer.SilenceCout();
    bool result = CorePtr()->GetCarManager().UpdateCar(licensePlate, car);
    silencer.RestoreCout();
    return result;
}

bool Boss::RemoveCar(const std::string& licensePlate)
{
    silencer.SilenceCout();
    bool result = CorePtr()->GetCarManager().RemoveCar(licensePlate);
    silencer.RestoreCout();
    return result;
}

std::optional<Interfaces::Car> Boss::GetCar(const std::string& licensePlate)
{
    silencer.SilenceCout();
    auto result = CorePtr()->GetCarManager().GetCar(licensePlate);
    silencer.RestoreCout();
    return result;
}

std::vector<Interfaces::Car> Boss::GetAllCars()
{
    silencer.SilenceCout();
    auto result = CorePtr()->GetCarManager().GetAllCars();
    silencer.RestoreCout();
    return result;
}

int API::Boss::GetCarMilage(const std::string& licensePlate)
{
    silencer.SilenceCout();
    int result = CorePtr()->GetCarManager().GetCarMilage(licensePlate);
    silencer.RestoreCout();
    return result;
}

bool API::User::BookCar(
    const Interfaces::User& user,
    const Interfaces::CarType& carType,
    const unsigned char& passengers,
    const Interfaces::CircleType& startCircle,
    const Interfaces::CircleType& endCircle,
    const int numOfDays)
{
    silencer.SilenceCout();
    bool result = CorePtr()->GetBookingManager().BookCar(user, carType, passengers, startCircle, endCircle, numOfDays);
    silencer.RestoreCout();
    return result;
}

std::vector<Interfaces::TripInfo> API::User::GetUserTrips(const std::string& drivingLicense)
{
    silencer.SilenceCout();
    auto result = CorePtr()->GetBookingManager().GetUserTrips(drivingLicense);
    silencer.RestoreCout();
    return result;
}

void API::Server::NextDay()
{
    silencer.SilenceCout();
    CorePtr()->NextDay();
    silencer.RestoreCout();
}

std::string API::Server::GetCurrentDate()
{
    return CorePtr()->GetCurrentDate();
}
