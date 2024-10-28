#include <iostream>
#include "StructureInterface.h"
#include "Server.h"
#include "Boss.h"
#include "User.h"
#include <filesystem>
#include "CoutSilencer.h"

using SERVER = Server* (*)();
SERVER ServerPtr = Server::GetInstance;

using BOSS = Boss* (*)();
BOSS BossPtr = Boss::GetInstance;

void RemoveDB()
{
    const std::string testDBPath = "database/database.db";
    if (std::filesystem::exists(testDBPath))
    {
        std::filesystem::remove(testDBPath);
    }
}

void ChangeDay()
{
    std::cout << "-----CHANGE DAY-----\n";
    ServerPtr()->NextDay();
    std::cout << "Current date: " << ServerPtr()->GetCurrentDate() << std::endl;
}

void PressEnterToContinue()
{
    std::cout << "\n\nPress Enter to continue...";
    std::cin.get();
}


int main()
{
    // Remove DB only for demo to show for example unregistered user actions
    RemoveDB();

    std::cout << "Current date: " << ServerPtr()->GetCurrentDate() << std::endl;
    PressEnterToContinue();

    // Land Rover Defender
    BossPtr()->RegisterCar(Interfaces::Car(
        Interfaces::CarType::DELUXE, "EF213FG", "Land Rover", "Defender", Interfaces::CarStatus::AVAILABLE));

    // Toyota Corolla
    BossPtr()->RegisterCar(Interfaces::Car(
        Interfaces::CarType::MID_CLASS, "AB123CD", "Toyota", "Corolla", Interfaces::CarStatus::AVAILABLE));

    // FIAT TOPOLINO
    BossPtr()->RegisterCar(
        Interfaces::Car(Interfaces::CarType::ECO, "XY123ZZ", "FIAT", "TOPOLINO", Interfaces::CarStatus::AVAILABLE));

    PressEnterToContinue();

    User RaffaeleBerzoini("AB123CD", "Raffaele", "Berzoini", "Via Viale 20", "1234 5678 9012 3456");
    RaffaeleBerzoini.RegisterToRentalService();

    User MarioRossi("EF213FG", "Mario", "Rossi", "Via Roma 10", "9623 4567 8901 2345");

    PressEnterToContinue();

    // Actions as unregistered user
    MarioRossi.VisualizeTrips();
    MarioRossi.BookCar(
        Interfaces::CarType::DELUXE, 4, Interfaces::CircleType::INNER_CIRCLE, Interfaces::CircleType::OUTER_CIRCLE, 3);

    PressEnterToContinue();

    // Actions as registered user
    MarioRossi.RegisterToRentalService();
    MarioRossi.BookCar(
        Interfaces::CarType::DELUXE, 4, Interfaces::CircleType::INNER_CIRCLE, Interfaces::CircleType::OUTER_CIRCLE, 3);
    MarioRossi.VisualizeTrips();

    PressEnterToContinue();

    // Trying to book the same car
    RaffaeleBerzoini.BookCar(
        Interfaces::CarType::DELUXE, 4, Interfaces::CircleType::INNER_CIRCLE, Interfaces::CircleType::OUTER_CIRCLE, 7);

    PressEnterToContinue();

    BossPtr()->CheckAllUsers();
    BossPtr()->CheckAllCars();

    PressEnterToContinue();

    // Raffaele Berzoini waits for the deluxe car to be available
    for (int i = 0; i < 4; i++)
    {
        ChangeDay();
        BossPtr()->CheckCar("EF213FG");
    }

    PressEnterToContinue();

    // Raffaele Berzoini books the car
    RaffaeleBerzoini.BookCar(
        Interfaces::CarType::DELUXE, 4, Interfaces::CircleType::INNER_CIRCLE, Interfaces::CircleType::MIDDLE_CIRCLE, 7);

    PressEnterToContinue();

    ChangeDay();
    // Boss rent the toyota for himself
    BossPtr()->ChangeCarAvailability("AB123CD", Interfaces::CarStatus::RENTED);

    PressEnterToContinue();

    // Mario Rossi change his address
    MarioRossi.UpdateAddress("Via Milano 30");
    BossPtr()->CheckAllUsers();

    PressEnterToContinue();

    // Mario Rossi tries to book the toyota
    MarioRossi.BookCar(
        Interfaces::CarType::MID_CLASS,
        4,
        Interfaces::CircleType::INNER_CIRCLE,
        Interfaces::CircleType::OUTER_CIRCLE,
        7);

    ChangeDay();

    PressEnterToContinue();

    // Boss return the toyota
    BossPtr()->ChangeCarAvailability("AB123CD", Interfaces::CarStatus::AVAILABLE);

    PressEnterToContinue();

    // Mario Rossi books the toyota with too many passengers
    MarioRossi.BookCar(
        Interfaces::CarType::MID_CLASS,
        5,
        Interfaces::CircleType::INNER_CIRCLE,
        Interfaces::CircleType::OUTER_CIRCLE,
        1);

    PressEnterToContinue();

    // Mario Rossi books the toyota
    MarioRossi.BookCar(
        Interfaces::CarType::MID_CLASS,
        4,
        Interfaces::CircleType::INNER_CIRCLE,
        Interfaces::CircleType::OUTER_CIRCLE,
        1);

    PressEnterToContinue();

    // Let's cycle days until we need service for the toyota (>= 1500 km)

    ChangeDay();
    // Car returns available
    ChangeDay();

    // Check current car mileage (15km)
    BossPtr()->CheckCar("AB123CD");

    // Let's cycle days until we need service for the toyota (>= 1500 km)
    for (int i = 0; i < 98; i++)
    {
        CoutSilencer silencer;
        silencer.SilenceCout();
        RaffaeleBerzoini.BookCar(
            Interfaces::CarType::MID_CLASS,
            4,
            Interfaces::CircleType::INNER_CIRCLE,
            Interfaces::CircleType::OUTER_CIRCLE,
            0);
        ChangeDay();
        silencer.RestoreCout();
    }

    // Check current car mileage (1485)
    BossPtr()->CheckCar("AB123CD");

    PressEnterToContinue();

    // Another booking for the toyota
    MarioRossi.BookCar(
        Interfaces::CarType::MID_CLASS,
        4,
        Interfaces::CircleType::INNER_CIRCLE,
        Interfaces::CircleType::OUTER_CIRCLE,
        0);

    BossPtr()->CheckCar("AB123CD");

    PressEnterToContinue();

    ChangeDay();

    // Check current car mileage (1500)
    BossPtr()->CheckCar("AB123CD");

    // Car Under Service
    RaffaeleBerzoini.BookCar(
        Interfaces::CarType::MID_CLASS,
        4,
        Interfaces::CircleType::INNER_CIRCLE,
        Interfaces::CircleType::OUTER_CIRCLE,
        1);

    PressEnterToContinue();

    ChangeDay();

    // Check current car status
    BossPtr()->CheckCar("AB123CD");

    PressEnterToContinue();

    // Car available again
    RaffaeleBerzoini.BookCar(
        Interfaces::CarType::MID_CLASS,
        4,
        Interfaces::CircleType::INNER_CIRCLE,
        Interfaces::CircleType::OUTER_CIRCLE,
        1);


    PressEnterToContinue();

    // Removing a user and a car
    BossPtr()->RemoveUser("AB123CD");
    BossPtr()->RemoveCar("AB123CD");
    BossPtr()->CheckAllUsers();
    BossPtr()->CheckAllCars();

    PressEnterToContinue();

    return 0;
}
