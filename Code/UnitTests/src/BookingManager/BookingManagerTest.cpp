#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "BookingManager.h"
#include <filesystem>

TEST_CASE("BookingManagerTest")
{
    const std::string testDBPath = "database/test_db.db";

    // Remove any existing test database file to ensure a clean start
    if (std::filesystem::exists(testDBPath))
    {
        std::filesystem::remove(testDBPath);
    }

    auto dbManager = std::make_shared<DatabaseManager>(testDBPath);
    BookingManager bookingManager(dbManager);

    /*
    bool BookCar(
        const Interfaces::User& user,
        const Interfaces::CarType& carType,
        const unsigned char& passengers,
        const Interfaces::CircleType& startCircle,
        const Interfaces::CircleType& endCircle,
        const int numOfDays);
    */
    SECTION("No cars added to the DB")
    {
        Interfaces::User user("John", "Doe", "123 Main St", "1234 5678 9012 3456", "AA0011");
        Interfaces::CarType carType = Interfaces::CarType::DELUXE;
        unsigned char passengers = 4;
        Interfaces::CircleType startCircle = Interfaces::CircleType::INNER_CIRCLE;
        Interfaces::CircleType endCircle = Interfaces::CircleType::MIDDLE_CIRCLE;
        int numOfDays = 3;

        REQUIRE_FALSE(bookingManager.BookCar(user, carType, passengers, startCircle, endCircle, numOfDays));
    }

    SECTION("Cars added to DB but no user registered")
    {
        Interfaces::Car car1(Interfaces::CarType::ECO, "ABC123", "Toyota", "Corolla", Interfaces::CarStatus::AVAILABLE);
        Interfaces::Car car2(Interfaces::CarType::DELUXE, "DEF456", "Honda", "Civic", Interfaces::CarStatus::AVAILABLE);
        dbManager->AddCar(car1);
        dbManager->AddCar(car2);

        Interfaces::User user("John", "Doe", "123 Main St", "1234 5678 9012 3456", "AA0011");
        Interfaces::CarType carType = Interfaces::CarType::DELUXE;
        unsigned char passengers = 4;
        Interfaces::CircleType startCircle = Interfaces::CircleType::INNER_CIRCLE;
        Interfaces::CircleType endCircle = Interfaces::CircleType::MIDDLE_CIRCLE;
        int numOfDays = 3;

        REQUIRE_FALSE(bookingManager.BookCar(user, carType, passengers, startCircle, endCircle, numOfDays));
    }

    SECTION("Cars added to DB and user registered")
    {
        Interfaces::Car car1(Interfaces::CarType::ECO, "ABC123", "Toyota", "Corolla", Interfaces::CarStatus::AVAILABLE);
        Interfaces::Car car2(Interfaces::CarType::DELUXE, "DEF456", "Honda", "Civic", Interfaces::CarStatus::AVAILABLE);
        dbManager->AddCar(car1);
        dbManager->AddCar(car2);

        Interfaces::User user("John", "Doe", "123 Main St", "1234 5678 9012 3456", "AA0011");
        dbManager->AddUser(user);

        Interfaces::CarType carType = Interfaces::CarType::DELUXE;
        unsigned char passengers = 4;
        Interfaces::CircleType startCircle = Interfaces::CircleType::INNER_CIRCLE;
        Interfaces::CircleType endCircle = Interfaces::CircleType::MIDDLE_CIRCLE;
        int numOfDays = 3;

        REQUIRE(bookingManager.BookCar(user, carType, passengers, startCircle, endCircle, numOfDays));
        
        Interfaces::TripInfo tripInfo = dbManager->GetUserTrips("AA0011")[0];
        REQUIRE(tripInfo.trip.start_circle == startCircle);
        REQUIRE(tripInfo.trip.destination_circle == endCircle);
        REQUIRE(tripInfo.trip.distance == 10);
        REQUIRE(tripInfo.trip.cost == 50);
        REQUIRE(tripInfo.trip.start_rental_date == dbManager->GetCurrentDate());
        REQUIRE(tripInfo.trip.end_rental_date == dbManager->GetNextDate(numOfDays));
        REQUIRE(tripInfo.car.carSpecifics.type == carType);
        REQUIRE(tripInfo.car.carSpecifics.pricePerKm == 5);
        REQUIRE(tripInfo.car.carSpecifics.seats == 7);
        REQUIRE(tripInfo.car.carSpecifics.speed == 50);
        REQUIRE(tripInfo.car.licensePlate == "DEF456");
        REQUIRE(tripInfo.car.brand == "Honda");
        REQUIRE(tripInfo.car.name == "Civic");
        REQUIRE(tripInfo.car.status == Interfaces::CarStatus::RENTED);

        REQUIRE(tripInfo.user == user);
    }

    SECTION("Cars and user registered - Extra passengers")
    {
        Interfaces::Car car1(Interfaces::CarType::ECO, "ABC123", "Toyota", "Corolla", Interfaces::CarStatus::AVAILABLE);
        Interfaces::Car car2(Interfaces::CarType::DELUXE, "DEF456", "Honda", "Civic", Interfaces::CarStatus::AVAILABLE);
        dbManager->AddCar(car1);
        dbManager->AddCar(car2);

        Interfaces::User user("John", "Doe", "123 Main St", "1234 5678 9012 3456", "AA0011");
        dbManager->AddUser(user);

        Interfaces::CarType carType = Interfaces::CarType::ECO;
        unsigned char passengers = 4;
        Interfaces::CircleType startCircle = Interfaces::CircleType::INNER_CIRCLE;
        Interfaces::CircleType endCircle = Interfaces::CircleType::MIDDLE_CIRCLE;
        int numOfDays = 3;

        REQUIRE_FALSE(bookingManager.BookCar(user, carType, passengers, startCircle, endCircle, numOfDays));
    }
}