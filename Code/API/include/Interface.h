#include <vector>
#include <optional>
#include "StructureInterface.h"

#ifdef INTERFACE_EXPORT
#    define API_INTERFACE_EXPORT __declspec(dllexport)
#else
#    define API_INTERFACE_EXPORT __declspec(dllimport)
#endif

namespace API
{
	namespace Common
	{
        API_INTERFACE_EXPORT bool AddUser(const Interfaces::User& user);
        API_INTERFACE_EXPORT bool UpdateUser(const std::string& driving_license, const Interfaces::User& user);
        API_INTERFACE_EXPORT bool RemoveUser(const std::string& drivingLicense);
        API_INTERFACE_EXPORT std::optional<Interfaces::User> GetUser(const std::string& drivingLicense);
	}

	namespace Boss
	{
        API_INTERFACE_EXPORT std::vector<Interfaces::User> GetAllUsers();
        API_INTERFACE_EXPORT bool AddCar(const Interfaces::Car& car);
        API_INTERFACE_EXPORT bool UpdateCar(const std::string& licensePlate, const Interfaces::Car& car);
        API_INTERFACE_EXPORT bool RemoveCar(const std::string& licensePlate);
        API_INTERFACE_EXPORT std::optional<Interfaces::Car> GetCar(const std::string& licensePlate);
        API_INTERFACE_EXPORT std::vector<Interfaces::Car> GetAllCars();
	}

	namespace User
    {
        API_INTERFACE_EXPORT bool BookCar(
            const Interfaces::User& user,
            const Interfaces::CarType& carType,
            const unsigned char& passengers,
            const Interfaces::CircleType& startCircle,
            const Interfaces::CircleType& endCircle,
            const int numOfDays);
        API_INTERFACE_EXPORT std::vector<Interfaces::TripInfo> GetUserTrips(const std::string& drivingLicense);
	}

    namespace Server
    {
        API_INTERFACE_EXPORT void NextDay();
        API_INTERFACE_EXPORT std::string GetCurrentDate();
    }

}