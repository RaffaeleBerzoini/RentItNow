#include "DatabaseManager.h"

class CarManager
{
private:
    std::shared_ptr<DatabaseManager> dbManager;

public:
    bool AddCar(const Interfaces::Car& car);
    bool UpdateCar(const std::string& licensePlate, const Interfaces::Car& car);
    bool RemoveCar(const std::string& licensePlate);
    std::optional<Interfaces::Car> GetCar(const std::string& drivingLicense);
    std::vector<Interfaces::Car> GetAllCars();

    CarManager(std::shared_ptr<DatabaseManager> dbManagerPtr);
};