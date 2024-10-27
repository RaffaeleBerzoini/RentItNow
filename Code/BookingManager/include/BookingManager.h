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

    Interfaces::Trip GetTrip(const std::string& drivingLicense);

    BookingManager(std::shared_ptr<DatabaseManager> dbManagerPtr);
};