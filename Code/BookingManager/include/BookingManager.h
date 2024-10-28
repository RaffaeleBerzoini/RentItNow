#include "DatabaseManager.h"

class BookingManager
{
private:
    std::shared_ptr<DatabaseManager> dbManager;

    std::vector<Interfaces::Car> GetAvailableCars();
    std::optional<Interfaces::Car> GetBestCar(        
        const Interfaces::CarType& carType,
        const unsigned char& passengers);

    int ComputeDistance(const Interfaces::CircleType& startCircle, const Interfaces::CircleType& endCircle);
    double ComputePrice(const Interfaces::CarSpecifics& carSpecifics, const int& distance);

public:

    bool BookCar(
        const Interfaces::User& user,
        const Interfaces::CarType& carType,
        const unsigned char& passengers,
        const Interfaces::CircleType& startCircle,
        const Interfaces::CircleType& endCircle,        
        const int numOfDays);

    // Methods to get all user trips, filtered by type, etc.
    std::vector<Interfaces::TripInfo> GetUserTrips(const std::string& drivingLicense);
    std::vector<Interfaces::TripInfo> GetUserTrips(
        const std::string& drivingLicense,
        const Interfaces::CarType& carType);
    std::vector<Interfaces::TripInfo> GetUserTrips(
        const std::string& drivingLicense,
        const Interfaces::CircleType& startCircle,
        const Interfaces::CircleType& endCircle);
    std::vector<Interfaces::TripInfo> GetUserTrips(
        const std::string& drivingLicense,
        const Interfaces::CarType& carType,
        const Interfaces::CircleType& startCircle,
        const Interfaces::CircleType& endCircle);
    // start and end rental date
    std::vector<Interfaces::TripInfo> GetUserTrips(
        const std::string& drivingLicense,
        const std::string& startRentalDate,
        const std::string& endRentalDate);
    std::vector<Interfaces::TripInfo> GetUserTrips(
        const std::string& drivingLicense,
        const Interfaces::CarType& carType,
        const std::string& startRentalDate,
        const std::string& endRentalDate);
    std::vector<Interfaces::TripInfo> GetUserTrips(
        const std::string& drivingLicense,
        const Interfaces::CircleType& startCircle,
        const Interfaces::CircleType& endCircle,
        const std::string& startRentalDate,
        const std::string& endRentalDate);
    std::vector<Interfaces::TripInfo> GetUserTrips(
        const std::string& drivingLicense,
        const Interfaces::CarType& carType,
        const Interfaces::CircleType& startCircle,
        const Interfaces::CircleType& endCircle,
        const std::string& startRentalDate,
        const std::string& endRentalDate);

    

    BookingManager(std::shared_ptr<DatabaseManager> dbManagerPtr);
};