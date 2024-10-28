#ifndef BOSS_H
#define BOSS_H

#include "CoutSilencer.h"
#include "Interface.h"

class Boss : private CoutSilencer
{
private:
    Boss() = default;

    inline static Boss* instance = nullptr;

public:
    static Boss* GetInstance();

    void RegisterUser(Interfaces::User);
    void UpdateUser(const std::string& drivingLicense, Interfaces::User);
    void RemoveUser(const std::string& drivingLicense);

    void RegisterCar(Interfaces::Car);
    void UpdateCar(const std::string& licensePlate, Interfaces::Car);
    void RemoveCar(const std::string& licensePlate);
    void ChangeCarAvailability(const std::string& licensePlate, Interfaces::CarStatus carStatus);
    
    void CheckAllUsers();
    void CheckAllCars();
    void CheckAvailableCars();
    void CheckRentedCars();
    void CheckUnderServiceCars();

    void CheckUser(const std::string& drivingLicense);
    void CheckCar(const std::string& licensePlate);
};

#endif // BOSS_H