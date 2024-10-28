#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "Core.h"
#include <filesystem>

using CORE = Core* (*)();
CORE CorePtr = Core::GetInstance;

TEST_CASE("Business Logic")
{
    // There will be only one section in this test case since Core is a singleton
    // and removing the database file will affect all tests since Core creates the database
    // inside its constructor which is called only once.

    const std::string testDBPath = "database/test_db.db";
    Core::SetDatabasePath(testDBPath);

    // Remove any existing test database file to ensure a clean start
    if (std::filesystem::exists(testDBPath))
    {
        std::filesystem::remove(testDBPath);
    }

    std::optional<Interfaces::Service> serviceData;

    REQUIRE(!std::filesystem::exists(testDBPath));

    SECTION("Day2Day Logic")
    {
        REQUIRE(CorePtr() != nullptr);
        REQUIRE(std::filesystem::exists(testDBPath));
        REQUIRE(CorePtr() == CorePtr());

        // Add a user
        Interfaces::User user("John", "Doe", "123 Main St", "1234 5678 9012 3456", "AA0011");
        REQUIRE(CorePtr()->GetUserManager().AddUser(user));

        // Add a car
        Interfaces::Car car1(Interfaces::CarType::ECO, "ABC123", "Toyota", "Corolla", Interfaces::CarStatus::AVAILABLE);
        REQUIRE(CorePtr()->GetCarManager().AddCar(car1));
        // Check service
        serviceData = CorePtr()->GetCarManager().GetService("ABC123");
        REQUIRE(serviceData.has_value());
        REQUIRE(serviceData->service_date == "2024-11-01");
        REQUIRE(serviceData->distance_since_last_service == 0);

        // Book a trip
        Interfaces::CarType desiredCarType = Interfaces::CarType::DELUXE;
        unsigned char passengers = 2;
        Interfaces::CircleType startCircle = Interfaces::CircleType::INNER_CIRCLE;
        Interfaces::CircleType endCircle = Interfaces::CircleType::INNER_CIRCLE;
        int numOfDays = 5;
        // Fail to book a car since there are no deluxe cars available. Check toyota is still available
        REQUIRE_FALSE(CorePtr()->GetBookingManager().BookCar(
            user, desiredCarType, passengers, startCircle, endCircle, numOfDays));
        REQUIRE(CorePtr()->GetCarManager().GetCar("ABC123")->status == Interfaces::CarStatus::AVAILABLE);

        desiredCarType = Interfaces::CarType::ECO;
        // Book a car
        REQUIRE(CorePtr()->GetBookingManager().BookCar(
            user, desiredCarType, passengers, startCircle, endCircle, numOfDays));

        Interfaces::TripInfo tripInfo = CorePtr()->GetBookingManager().GetUserTrips("AA0011")[0];
        REQUIRE(tripInfo.trip.start_circle == startCircle);
        REQUIRE(tripInfo.trip.destination_circle == endCircle);
        REQUIRE(tripInfo.trip.distance == 5);
        REQUIRE(tripInfo.trip.cost == 5);
        REQUIRE(tripInfo.trip.start_rental_date == "2024-11-01");
        REQUIRE(tripInfo.trip.end_rental_date == "2024-11-06");
        REQUIRE(tripInfo.car.carSpecifics.type == desiredCarType);
        REQUIRE(tripInfo.car.carSpecifics.pricePerKm == 1);
        REQUIRE(tripInfo.car.carSpecifics.seats == 2);
        REQUIRE(tripInfo.car.carSpecifics.speed == 15);
        REQUIRE(tripInfo.car.licensePlate == "ABC123");
        REQUIRE(tripInfo.car.brand == "Toyota");
        REQUIRE(tripInfo.car.name == "Corolla");
        REQUIRE(tripInfo.car.status == Interfaces::CarStatus::RENTED);

        // Fail to book a car since there are no eco cars available
        REQUIRE_FALSE(CorePtr()->GetBookingManager().BookCar(
            user, desiredCarType, passengers, startCircle, endCircle, numOfDays));

        // Check the car is still rented after some days
        CorePtr()->NextDay();
        REQUIRE(CorePtr()->GetCarManager().GetCar("ABC123")->status == Interfaces::CarStatus::RENTED);
        CorePtr()->NextDay();
        REQUIRE(CorePtr()->GetCarManager().GetCar("ABC123")->status == Interfaces::CarStatus::RENTED);
        CorePtr()->NextDay();
        REQUIRE(CorePtr()->GetCarManager().GetCar("ABC123")->status == Interfaces::CarStatus::RENTED);
        CorePtr()->NextDay();
        REQUIRE(CorePtr()->GetCarManager().GetCar("ABC123")->status == Interfaces::CarStatus::RENTED);
        CorePtr()->NextDay();
        REQUIRE(CorePtr()->GetCarManager().GetCar("ABC123")->status == Interfaces::CarStatus::RENTED);
        // Check service
        serviceData = CorePtr()->GetCarManager().GetService("ABC123");
        REQUIRE(serviceData.has_value());
        REQUIRE(serviceData->service_date == "2024-11-01");
        REQUIRE(serviceData->distance_since_last_service == 0);


        // Becomes available after the trip ends
        CorePtr()->NextDay();
        REQUIRE(CorePtr()->GetCarManager().GetCar("ABC123")->status == Interfaces::CarStatus::AVAILABLE);
        CorePtr()->NextDay();
        REQUIRE(CorePtr()->GetCarManager().GetCar("ABC123")->status == Interfaces::CarStatus::AVAILABLE);

        // Check service
        serviceData = CorePtr()->GetCarManager().GetService("ABC123");
        REQUIRE(serviceData.has_value());
        REQUIRE(serviceData->service_date == "2024-11-01");
        REQUIRE(serviceData->distance_since_last_service == 5);

        // Faster testing
        endCircle = Interfaces::CircleType::OUTER_CIRCLE;
        // Book multiple trips untile we almost reach the service limit (1495km)
        for (int i = 0; i < 99; i++)
        {
            REQUIRE(
                CorePtr()->GetBookingManager().BookCar(user, desiredCarType, passengers, startCircle, endCircle, 1));
            CorePtr()->NextDay();
            REQUIRE_FALSE(
                CorePtr()->GetBookingManager().BookCar(user, desiredCarType, passengers, startCircle, endCircle, 1));
            CorePtr()->NextDay();
            serviceData = CorePtr()->GetCarManager().GetService("ABC123");
            REQUIRE(serviceData.has_value());
            REQUIRE(serviceData->service_date == "2024-11-01");
            REQUIRE(serviceData->distance_since_last_service == 20 + 15 * i);
        }

        // Now we are at 1490km, the car should be still available for booking
        REQUIRE(CorePtr()->GetCarManager().GetCar("ABC123")->status == Interfaces::CarStatus::AVAILABLE);

        // Book a trip that will reach the service limit
        REQUIRE(CorePtr()->GetBookingManager().BookCar(user, desiredCarType, passengers, startCircle, endCircle, 5));
        CorePtr()->NextDay();
        REQUIRE(CorePtr()->GetCarManager().GetCar("ABC123")->status == Interfaces::CarStatus::RENTED);
        CorePtr()->NextDay();
        REQUIRE(CorePtr()->GetCarManager().GetCar("ABC123")->status == Interfaces::CarStatus::RENTED);
        CorePtr()->NextDay();
        REQUIRE(CorePtr()->GetCarManager().GetCar("ABC123")->status == Interfaces::CarStatus::RENTED);
        CorePtr()->NextDay();
        REQUIRE(CorePtr()->GetCarManager().GetCar("ABC123")->status == Interfaces::CarStatus::RENTED);
        CorePtr()->NextDay();
        REQUIRE(CorePtr()->GetCarManager().GetCar("ABC123")->status == Interfaces::CarStatus::RENTED);


        // Check car status and service after 6 days
        CorePtr()->NextDay();
        REQUIRE(CorePtr()->GetCarManager().GetCar("ABC123")->status == Interfaces::CarStatus::UNDER_SERVICE);
        serviceData = CorePtr()->GetCarManager().GetService("ABC123");
        REQUIRE(serviceData.has_value());
        REQUIRE(serviceData->service_date == "2025-05-31");
        REQUIRE(serviceData->distance_since_last_service == 0);

        // Check car status and service after 1 day
        CorePtr()->NextDay();
        REQUIRE(CorePtr()->GetCarManager().GetCar("ABC123")->status == Interfaces::CarStatus::AVAILABLE);
        serviceData = CorePtr()->GetCarManager().GetService("ABC123");
        REQUIRE(serviceData.has_value());
        REQUIRE(serviceData->service_date == "2025-05-31");
        REQUIRE(serviceData->distance_since_last_service == 0);

    }
}