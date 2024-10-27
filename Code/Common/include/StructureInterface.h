#if !defined STRUCTURE_INTERFACE
#    define STRUCTURE_INTERFACE

#    include <string>
#    include <stdexcept>

enum class CarType
{
    ECO,
    MID_CLASS,
    DELUXE
};

enum class CarStatus
{
    AVAILABLE,
    RENTED,
    UNDER_SERVICE
};

enum class CircleType
{
    INNER_CIRCLE,
    MIDDLE_CIRCLE,
    OUTER_CIRCLE
};

struct User
{
    std::string name;
    std::string surname;
    std::string address;
    std::string creditCard;
    std::string drivingLicense;

    // Constructor only with non empty strings
    User(
        const std::string& name,
        const std::string& surname,
        const std::string& address,
        const std::string& creditCard,
        const std::string& drivingLicense)
    {
        if (name.empty() || surname.empty() || address.empty() || creditCard.empty() || drivingLicense.empty())
            throw std::invalid_argument("Empty string in User constructor");
        this->name = name;
        this->surname = surname;
        this->address = address;
        this->creditCard = creditCard;
        this->drivingLicense = drivingLicense;
    }
};

/*
There are three types of car:

ECO, max 2 persons,
MID-CLASS, max 4 persons,
DELUXE, max 7 persons.
Each type of car has a rental price per km:

ECO: 1$/km;
MID-CLASS: 2$/km;
DELUXE: 5$/km
Each type of car has a fixed speed:

ECO: 15km/h
MID-CLASS: 25km/h
DELUXE: 50km/h
*/

struct CarSpecifics
{
    unsigned char seats;
    unsigned char pricePerKm;
    unsigned char speed;
    CarType type;

    CarSpecifics(CarType type)
        : type(type)
    {
        switch (type)
        {
        case CarType::ECO:
            seats = 2;
            pricePerKm = 1;
            speed = 15;
            break;
        case CarType::MID_CLASS:
            seats = 4;
            pricePerKm = 2;
            speed = 25;
            break;
        case CarType::DELUXE:
            seats = 7;
            pricePerKm = 5;
            speed = 50;
            break;
        }
    }

    unsigned char seats() const
    {
        return seats;
    }

    unsigned char pricePerKm() const
    {
        return pricePerKm;
    }

    unsigned char speed() const
    {
        return speed;
    }
};

struct Car
{
    CarSpecifics carSpecifics;
    std::string licensePlate;
    std::string brand;
    std::string name;
    CarStatus status;

    std::string carTypeToString() const
    {
        switch (carSpecifics.type)
        {
        case CarType::ECO:
            return "ECO";
        case CarType::MID_CLASS:
            return "MID-CLASS";
        case CarType::DELUXE:
            return "DELUXE";
        }
        throw std::invalid_argument("Invalid CarType");
    }

    std::string carStatusToString() const
    {
        switch (status)
        {
        case CarStatus::AVAILABLE:
            return "available";
        case CarStatus::RENTED:
            return "rented";
        case CarStatus::UNDER_SERVICE:
            return "under_service";
        }
        throw std::invalid_argument("Invalid CarStatus");
    }

    CarType stringToCarType(const std::string& type) const
    {
        if (type == "ECO")
            return CarType::ECO;
        if (type == "MID-CLASS")
            return CarType::MID_CLASS;
        if (type == "DELUXE")
            return CarType::DELUXE;
        throw std::invalid_argument("Invalid car type string");
    }

    CarStatus stringToCarStatus(const std::string& status) const
    {
        if (status == "available")
            return CarStatus::AVAILABLE;
        if (status == "rented")
            return CarStatus::RENTED;
        if (status == "under_service")
            return CarStatus::UNDER_SERVICE;
        throw std::invalid_argument("Invalid car status string");
    }

    Car(CarType type,
        const std::string& licensePlate,
        const std::string& brand,
        const std::string& name,
        CarStatus status)
        : carSpecifics(type)
        , licensePlate(licensePlate)
        , brand(brand)
        , name(name)
        , status(status)
    {}

    Car(CarType type,
        const std::string& licensePlate,
        const std::string& brand,
        const std::string& name,
        const std::string& status)
        : carSpecifics(type)
        , licensePlate(licensePlate)
        , brand(brand)
        , name(name)
        , status(stringToCarStatus(status))
    {}

    Car(const std::string& type,
        const std::string& licensePlate,
        const std::string& brand,
        const std::string& name,
        const std::string& status)
        : carSpecifics(stringToCarType(type))
        , licensePlate(licensePlate)
        , brand(brand)
        , name(name)
        , status(stringToCarStatus(status))
    {}

    Car(const std::string& type,
        const std::string& licensePlate,
        const std::string& brand,
        const std::string& name,
        CarStatus status)
        : carSpecifics(stringToCarType(type))
        , licensePlate(licensePlate)
        , brand(brand)
        , name(name)
        , status(status)
    {}
};

struct Trip
{
    int user_id;
    int car_id;
    CircleType start_circle;
    CircleType destination_circle;
    int distance;
    double cost;
    std::string start_rental_date;
    std::string end_rental_date;

    std::string startCircleToString() const
    {
        return circleTypeToString(start_circle);
    }

    std::string destinationCircleToString() const
    {
        return circleTypeToString(destination_circle);
    }

    Trip(
        int user_id,
        int car_id,
        CircleType start_circle,
        CircleType destination_circle,
        int distance,
        double cost,
        const std::string& start_rental_date,
        const std::string& end_rental_date)
        : user_id(user_id)
        , car_id(car_id)
        , start_circle(start_circle)
        , destination_circle(destination_circle)
        , distance(distance)
        , cost(cost)
        , start_rental_date(start_rental_date)
        , end_rental_date(end_rental_date)
    {}

    Trip(
        int user_id,
        int car_id,
        const std::string& start_circle,
        const std::string& destination_circle,
        int distance,
        double cost,
        const std::string& start_rental_date,
        const std::string& end_rental_date)
        : user_id(user_id)
        , car_id(car_id)
        , start_circle(stringToCircleType(start_circle))
        , destination_circle(stringToCircleType(destination_circle))
        , distance(distance)
        , cost(cost)
        , start_rental_date(start_rental_date)
        , end_rental_date(end_rental_date)
    {}

    Trip(
        int user_id,
        int car_id,
        const std::string& start_circle,
        CircleType destination_circle,
        int distance,
        double cost,
        const std::string& start_rental_date,
        const std::string& end_rental_date)
        : user_id(user_id)
        , car_id(car_id)
        , start_circle(stringToCircleType(start_circle))
        , destination_circle(destination_circle)
        , distance(distance)
        , cost(cost)
        , start_rental_date(start_rental_date)
        , end_rental_date(end_rental_date)
    {}

    Trip(
        int user_id,
        int car_id,
        CircleType start_circle,
        const std::string& destination_circle,
        int distance,
        double cost,
        const std::string& start_rental_date,
        const std::string& end_rental_date)
        : user_id(user_id)
        , car_id(car_id)
        , start_circle(start_circle)
        , destination_circle(stringToCircleType(destination_circle))
        , distance(distance)
        , cost(cost)
        , start_rental_date(start_rental_date)
        , end_rental_date(end_rental_date)
    {}

private:

    std::string circleTypeToString(CircleType circle) const
    {
        switch (circle)
        {
        case CircleType::INNER_CIRCLE:
            return "Inner Circle";
        case CircleType::MIDDLE_CIRCLE:
            return "Middle Circle";
        case CircleType::OUTER_CIRCLE:
            return "Outer Circle";
        }
        throw std::invalid_argument("Invalid CircleType");
    }

    CircleType stringToCircleType(const std::string& circle) const
    {
        if (circle == "Inner Circle")
            return CircleType::INNER_CIRCLE;
        if (circle == "Middle Circle")
            return CircleType::MIDDLE_CIRCLE;
        if (circle == "Outer Circle")
            return CircleType::OUTER_CIRCLE;
        throw std::invalid_argument("Invalid circle type string");
    }
};


#endif