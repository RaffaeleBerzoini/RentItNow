#include "Boss.h"

Boss* Boss::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new Boss();
    }
    return instance;
}

void Boss::RegisterUser(Interfaces::User user)
{
    if (API::Common::AddUser(user))
    {
        std::cout << "User registered successfully" << std::endl;
    }
    else
    {
        std::cout << "User already registered" << std::endl;
    }
}

void Boss::UpdateUser(const std::string& drivingLicense, Interfaces::User user)
{
    if (API::Common::UpdateUser(drivingLicense, user))
    {
        std::cout << "User updated successfully" << std::endl;
    }
    else
    {
        std::cout << "User not found" << std::endl;
    }
}

void Boss::RemoveUser(const std::string& drivingLicense)
{
    if (API::Common::RemoveUser(drivingLicense))
    {
        std::cout << "User removed successfully" << std::endl;
    }
    else
    {
        std::cout << "User not found" << std::endl;
    }
}

void Boss::RegisterCar(Interfaces::Car car)
{
    if (API::Boss::AddCar(car))
    {
        std::cout << "Car registered successfully" << std::endl;
    }
    else
    {
        std::cout << "Car already registered" << std::endl;
    }
}

void Boss::UpdateCar(const std::string& licensePlate, Interfaces::Car car)
{
    if (API::Boss::UpdateCar(licensePlate, car))
    {
        std::cout << "Car updated successfully" << std::endl;
    }
    else
    {
        std::cout << "Car not found" << std::endl;
    }
}

void Boss::RemoveCar(const std::string& licensePlate)
{
    if (API::Boss::RemoveCar(licensePlate))
    {
        std::cout << "Car removed successfully" << std::endl;
    }
    else
    {
        std::cout << "Car not found" << std::endl;
    }
}

void Boss::ChangeCarAvailability(const std::string& licensePlate, Interfaces::CarStatus carStatus)
{
    Interfaces::Car car = API::Boss::GetCar(licensePlate).value();
    car.status = carStatus;
    UpdateCar(licensePlate, car);
}

void Boss::CheckAllUsers()
{
    for (const auto& user : API::Boss::GetAllUsers())
    {
        std::cout << "--------------------------------" << std::endl;
        std::cout << user.name << " " << user.surname << std::endl;
        std::cout << user.address << std::endl;
        std::cout << user.creditCard << std::endl;
        std::cout << user.drivingLicense << std::endl;
    }
}

void Boss::CheckAllCars()
{
    for (const auto& car : API::Boss::GetAllCars())
    {
        std::cout << "--------------------------------" << std::endl;
        std::cout << car.brand << " " << car.name << std::endl;
        std::cout << car.licensePlate << std::endl;
        std::cout << car.carStatusToString() << std::endl;
        std::cout << "Type: " << car.carTypeToString() << std::endl;
        std::cout << "Seats: "<< car.carSpecifics.seats << std::endl;
        std::cout << "PriceKm: "<< car.carSpecifics.pricePerKm << std::endl;
        std::cout << "Speed: "<< car.carSpecifics.speed << std::endl;
        std::cout << "Mileage: " << API::Boss::GetCarMilage(car.licensePlate) << " Km" << std::endl;
    }
}

void Boss::CheckAvailableCars()
{
    for (const auto& car : API::Boss::GetAllCars())
    {
        if (car.status == Interfaces::CarStatus::AVAILABLE)
        {
            std::cout << "--------------------------------" << std::endl;
            std::cout << car.brand << " " << car.name << std::endl;
            std::cout << car.licensePlate << std::endl;
            std::cout << car.carStatusToString() << std::endl;
            std::cout << "Type: " << car.carTypeToString() << std::endl;
            std::cout << "Seats: " << car.carSpecifics.seats << std::endl;
            std::cout << "PriceKm: " << car.carSpecifics.pricePerKm << std::endl;
            std::cout << "Speed: " << car.carSpecifics.speed << std::endl;
            std::cout << "Mileage: " << API::Boss::GetCarMilage(car.licensePlate) << " Km" << std::endl;
        }
    }
}

void Boss::CheckRentedCars()
{
    for (const auto& car : API::Boss::GetAllCars())
    {
        if (car.status == Interfaces::CarStatus::RENTED)
        {
            std::cout << "--------------------------------" << std::endl;
            std::cout << car.brand << " " << car.name << std::endl;
            std::cout << car.licensePlate << std::endl;
            std::cout << car.carStatusToString() << std::endl;
            std::cout << "Type: " << car.carTypeToString() << std::endl;
            std::cout << "Seats: " << car.carSpecifics.seats << std::endl;
            std::cout << "PriceKm: " << car.carSpecifics.pricePerKm << std::endl;
            std::cout << "Speed: " << car.carSpecifics.speed << std::endl;
            std::cout << "Mileage: " << API::Boss::GetCarMilage(car.licensePlate) << " Km" << std::endl;
        }
    }
}

void Boss::CheckUnderServiceCars()
{
    for (const auto& car : API::Boss::GetAllCars())
    {
        if (car.status == Interfaces::CarStatus::UNDER_SERVICE)
        {
            std::cout << "--------------------------------" << std::endl;
            std::cout << car.brand << " " << car.name << std::endl;
            std::cout << car.licensePlate << std::endl;
            std::cout << car.carStatusToString() << std::endl;
            std::cout << "Type: " << car.carTypeToString() << std::endl;
            std::cout << "Seats: " << car.carSpecifics.seats << std::endl;
            std::cout << "PriceKm: " << car.carSpecifics.pricePerKm << std::endl;
            std::cout << "Speed: " << car.carSpecifics.speed << std::endl;
            std::cout << "Mileage: " << API::Boss::GetCarMilage(car.licensePlate) << " Km" << std::endl;
        }
    }
}

void Boss::CheckUser(const std::string& drivingLicense)
{
    std::optional<Interfaces::User> user = API::Common::GetUser(drivingLicense);
    if (user.has_value())
    {
        std::cout << "--------------------------------" << std::endl;
        std::cout << user.value().name << " " << user.value().surname << std::endl;
        std::cout << user.value().address << std::endl;
        std::cout << user.value().creditCard << std::endl;
        std::cout << user.value().drivingLicense << std::endl;
    }
    else
    {
        std::cout << "User not found" << std::endl;
    }
}

void Boss::CheckCar(const std::string& licensePlate)
{
    std::optional<Interfaces::Car> car = API::Boss::GetCar(licensePlate);
    if (car.has_value())
    {
        std::cout << "--------------------------------" << std::endl;
        std::cout << car.value().brand << " " << car.value().name << std::endl;
        std::cout << car.value().licensePlate << std::endl;
        std::cout << car.value().carStatusToString() << std::endl;
        std::cout << "Type: " << car.value().carTypeToString() << std::endl;
        std::cout << "Seats: " << car.value().carSpecifics.seats << std::endl;
        std::cout << "PriceKm: " << car.value().carSpecifics.pricePerKm << std::endl;
        std::cout << "Speed: " << car.value().carSpecifics.speed << std::endl;
        std::cout << "Mileage: " << API::Boss::GetCarMilage(car.value().licensePlate) << " Km" << std::endl;
    }
    else
    {
        std::cout << "Car not found" << std::endl;
    }
}