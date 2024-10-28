#ifndef USER_H
#define USER_H

#include "Interface.h"
#include <string>
#include <iostream>

class User
{
private:
    Interfaces::User user;

    // Check if user is registered. 
    // Please note that API and underlyiing implementation case where user is not registered. 
    // This is just a helper function for cleaner output messages.
    bool IsRegistered();

public:
    User(
        const std::string& drivingLicense,
        const std::string& name,
        const std::string& surname,
        const std::string& address,
        const std::string& creditCard);

    void RegisterToRentalService();
    void UpdateAddress(const std::string& address);
    void UpdateCreditCard(const std::string& creditCard);
    void UnregisterFromRentalService();

    void BookCar(
        const Interfaces::CarType& carType,
        const unsigned char& passengers,
        const Interfaces::CircleType& startCircle,
        const Interfaces::CircleType& endCircle,
        const int numOfDays);

    void VisualizeTrips();
};

#endif // USER_H
