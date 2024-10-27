#include "DatabaseManager.h"

class BookingManager
{
private:
    std::shared_ptr<DatabaseManager> dbManager;

    std::vector<Interfaces::Car> GetAvailableCars();
    std::optional<Interfaces::Car> GetBestCar(
        const std::vector<Interfaces::Car>& cars,
        const Interfaces::CarType& carType,
        const unsigned char& passengers);

public:

    bool BookCar(
        const Interfaces::User& user,
        const Interfaces::CarType& carType,
        const unsigned char& passengers,
        const Interfaces::CircleType& startCircle,
        const Interfaces::CircleType& endCircle,
        const std::string& startDate,
        const std::string& endDate);

    Interfaces::Trip GetTrip(const std::string& drivingLicense);

    BookingManager(std::shared_ptr<DatabaseManager> dbManagerPtr);
};