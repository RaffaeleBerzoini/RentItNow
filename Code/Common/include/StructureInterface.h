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
    unsigned char _seats;
    unsigned char price_per_km;
    unsigned char speed;
    CarType type;

    CarSpecifics(CarType type)
        : type(type)
    {
        switch (type)
        {
        case CarType::ECO:
            _seats = 2;
            price_per_km = 1;
            speed = 15;
            break;
        case CarType::MID_CLASS:
            _seats = 4;
            price_per_km = 2;
            speed = 25;
            break;
        case CarType::DELUXE:
            _seats = 7;
            price_per_km = 5;
            speed = 50;
            break;
        }
    }

    unsigned char seats() const
    {
        return _seats;
    }

    unsigned char pricePerKm() const
    {
        return price_per_km;
    }

    unsigned char speed() const
    {
        return speed;
    }
};

struct Car
{
    CarSpecifics carSpecifics;
    std::string license_plate;
    std::string brand;
    std::string name;
    CarStatus status;

    std::string carTypeToString()
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

    std::string carStatusToString()
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

    CarType stringToCarType(const std::string& type)
    {
        if (type == "ECO")
            return CarType::ECO;
        if (type == "MID-CLASS")
            return CarType::MID_CLASS;
        if (type == "DELUXE")
            return CarType::DELUXE;
        throw std::invalid_argument("Invalid car type string");
    }

    CarStatus stringToCarStatus(const std::string& status)
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
        const std::string& license_plate,
        const std::string& brand,
        const std::string& name,
        CarStatus status)
        : carSpecifics(type)
        , license_plate(license_plate)
        , brand(brand)
        , name(name)
        , status(status)
    {}

    Car(CarType type,
        const std::string& license_plate,
        const std::string& brand,
        const std::string& name,
        const std::string& status)
        : carSpecifics(type)
        , license_plate(license_plate)
        , brand(brand)
        , name(name)
        , status(stringToCarStatus(status))
    {}

    Car(const std::string& type,
        const std::string& license_plate,
        const std::string& brand,
        const std::string& name,
        const std::string& status)
        : carSpecifics(stringToCarType(type))
        , license_plate(license_plate)
        , brand(brand)
        , name(name)
        , status(stringToCarStatus(status))
    {}

    Car(const std::string& type,
        const std::string& license_plate,
        const std::string& brand,
        const std::string& name,
        CarStatus status)
        : carSpecifics(stringToCarType(type))
        , license_plate(license_plate)
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

    std::string startCircleToString()
    {
        return circleTypeToString(start_circle);
    }

    std::string destinationCircleToString()
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

    std::string circleTypeToString(CircleType circle)
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

    CircleType stringToCircleType(const std::string& circle)
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