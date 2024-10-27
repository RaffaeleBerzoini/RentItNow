#include <iostream>
#include "StructureInterface.h"
#include "Interface.h"

int main()
{
    // TODO: threads/classes for boss and user consoles

    // Boss logic users
    Interfaces::User user("Raffa", "Berzo", "123 Main St", "1234 5678 9012 3456", "AA0011");
    API::Common::AddUser(user);

    user.address = "456 Main St";
    API::Common::AddUser(user);

    Interfaces::User user2("Mary", "Jane", "789 Main St", "1234 5678 9999 3456", "AA0012");
    API::Common::AddUser(user2);

    auto users = API::Common::GetAllUsers();

    for (const auto& user : users)
    {
        std::cout << user.name << " " << user.surname << " " << user.address << " " << user.drivingLicense << " "
                  << user.creditCard << std::endl;
    }

    // Modify user
    user.creditCard = "6543 2109 8765 4321";
    API::Common::UpdateUser("AA0011", user);

    // Remove user
    API::Common::RemoveUser("AA0012");

    users = API::Common::GetAllUsers();

    for (const auto& user : users)
    {
        std::cout << user.name << " " << user.surname << " " << user.address << " " << user.drivingLicense << " "
                  << user.creditCard << std::endl;
    }

    // Boss logic cars
    Interfaces::Car car(Interfaces::CarType::ECO, "ABC123", "Toyota", "Corolla", Interfaces::CarStatus::AVAILABLE);
    API::Boss::AddCar(car);

    car.licensePlate = "DEF456";
    car.brand = "Ford";
    car.name = "Focus";
    car.carSpecifics.type = Interfaces::CarType::DELUXE;
    API::Boss::AddCar(car);

    car.licensePlate = "GHI789";
    car.brand = "Chevrolet";
    car.name = "Cruze";
    car.carSpecifics.type = Interfaces::CarType::MID_CLASS;
    API::Boss::AddCar(car);

    auto cars = API::Boss::GetAllCars();

    for (const auto& car : cars)
    {
        std::cout << car.licensePlate << " " << car.brand << " " << car.name << " " << car.carTypeToString()
                  << " " << car.carStatusToString() << std::endl;
    }

    // Modify car
    car.carSpecifics.type = Interfaces::CarType::ECO;
    car.status = Interfaces::CarStatus::RENTED;
    API::Boss ::UpdateCar("DEF456", car);

    // Remove car
    API::Boss ::RemoveCar("GHI789");

    cars = API::Boss::GetAllCars();

    for (const auto& car : cars)
    {
        std::cout << car.licensePlate << " " << car.brand << " " << car.name << " " << car.carTypeToString() << " "
                  << car.carStatusToString() << std::endl;
    }

    return 0;
}
