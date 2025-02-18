#include "DatabaseManager.h"
#include <iostream>
#include <filesystem>

namespace
{
std::string INITIAL_DATE = "2024-11-01"; // Initial date for the database when it's created
int MAX_DISTANCE = 1500;                 // Maximum distance a car can travel before needing a service
}

DatabaseManager::DatabaseManager(const std::string& dbFilePath)
    : dbFilePath(dbFilePath)
{
    std::lock_guard<std::mutex> lock(dbMutex);

    std::filesystem::path dbDirectory{};
    // Create a path object from the dbFilePath
    std::filesystem::path dbPath(dbFilePath);

    // Check if the directory path is empty
    if (!dbPath.has_parent_path() || dbPath.parent_path().empty())
    {
        throw std::runtime_error("Error: Database file path must include a directory.");
    }

    // Set dbDirectory to the parent directory
    dbDirectory = dbPath.parent_path();

    // Create the directory if it doesn't exist
    std::filesystem::create_directories(dbDirectory);

    SetupDB();
}

sqlite3* DatabaseManager::OpenDB()
{
    sqlite3* db;
    if (sqlite3_open(dbFilePath.c_str(), &db) != SQLITE_OK)
    {
        throw std::runtime_error("Error: Can't open database: " + std::string(sqlite3_errmsg(db)));
    }

    return db;
}

void DatabaseManager::SetupDB()
{
    // Open the database
    sqlite3* db = OpenDB();

    CreateTables(db);

    // Close the database connection
    sqlite3_close(db);
}

bool DatabaseManager::AddUser(const Interfaces::User& user)
{
    std::lock_guard<std::mutex> lock(dbMutex);

    sqlite3* db = OpenDB();

    const char* sql = R"(
            INSERT INTO Users (name, surname, address, credit_card, driving_license)
            VALUES (?, ?, ?, ?, ?);
        )";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare addUser statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, user.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, user.surname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, user.address.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, user.creditCard.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, user.drivingLicense.c_str(), -1, SQLITE_STATIC);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    if (!success)
    {
        std::cerr << "Failed to add user: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_close(db);
    return success;
}

// Update a user in the database
bool DatabaseManager::UpdateUser(const std::string& driving_license, const Interfaces::User& updatedUser)
{
    std::lock_guard<std::mutex> lock(dbMutex);

    sqlite3* db = OpenDB();

    const char* sql = R"(
            UPDATE Users SET
                name = ?,
                surname = ?,
                address = ?,
                credit_card = ?
            WHERE driving_license = ?;
        )";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare updateUser statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_text(stmt, 1, updatedUser.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, updatedUser.surname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, updatedUser.address.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, updatedUser.creditCard.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, driving_license.c_str(), -1, SQLITE_STATIC);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    if (!success)
    {
        std::cerr << "Failed to update user: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_close(db);
    return success;
}

// Remove a user from the database
bool DatabaseManager::RemoveUser(const std::string& driving_license)
{
    std::lock_guard<std::mutex> lock(dbMutex);

    sqlite3* db = OpenDB();

    const char* sql = R"(
            DELETE FROM Users WHERE driving_license = ?;
        )";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare removeUser statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_text(stmt, 1, driving_license.c_str(), -1, SQLITE_STATIC);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    if (!success)
    {
        std::cerr << "Failed to remove user: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_close(db);
    return success;
}

std::optional<Interfaces::User> DatabaseManager::GetUser(const std::string& drivingLicense)
{
    std::lock_guard<std::mutex> lock(dbMutex);

    sqlite3* db = OpenDB();

    const char* sql = R"(
			SELECT name, surname, address, credit_card, driving_license
			FROM Users
			WHERE driving_license = ?;
		)";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare getUser statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return std::nullopt;
    }

    sqlite3_bind_text(stmt, 1, drivingLicense.c_str(), -1, SQLITE_STATIC);

    std::string name;
    std::string surname;
    std::string address;
    std::string creditCard;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        surname = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        address = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        creditCard = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
    }
    else
    {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return std::nullopt;
    }

    sqlite3_finalize(stmt);

    sqlite3_close(db);

    return Interfaces::User(name, surname, address, creditCard, drivingLicense);
}

std::vector<Interfaces::User> DatabaseManager::GetAllUsers()
{
    std::lock_guard<std::mutex> lock(dbMutex);

    sqlite3* db = OpenDB();

    const char* sql = R"(
			SELECT name, surname, address, credit_card, driving_license
			FROM Users;
		)";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare getAllUsers statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return std::vector<Interfaces::User>();
    }

    std::vector<Interfaces::User> users;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        std::string name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        std::string surname = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        std::string address = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        std::string creditCard = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        std::string drivingLicense = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));

        users.push_back(Interfaces::User(name, surname, address, creditCard, drivingLicense));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return users;
}

int DatabaseManager::GetUserID(const std::string& drivingLicense)
{
    std::lock_guard<std::mutex> lock(dbMutex);

    sqlite3* db = OpenDB();

    const char* sql = R"(
			SELECT id
			FROM Users
			WHERE driving_license = ?;
		)";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare getUserID statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return -1;
    }

    sqlite3_bind_text(stmt, 1, drivingLicense.c_str(), -1, SQLITE_STATIC);

    int id = -1;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        id = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return id;
}

bool DatabaseManager::AddCar(const Interfaces::Car& car)
{
    std::lock_guard<std::mutex> lock(dbMutex);

    sqlite3* db = OpenDB();


    const char* sql = R"(
			INSERT INTO Cars (car_type, license_plate, brand, name, status)
			VALUES (?, ?, ?, ?, ?);
		)";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare addCar statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Solving lifetime issuse
    std::string type = car.carTypeToString();
    std::string status = car.carStatusToString();

    sqlite3_bind_text(stmt, 1, type.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, car.licensePlate.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, car.brand.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, car.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, status.c_str(), -1, SQLITE_STATIC);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    if (!success)
    {
        std::cerr << "Failed to add car: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_close(db);

    AddService(car.licensePlate);

    return success;
}

bool DatabaseManager::UpdateCar(const std::string& licensePlate, const Interfaces::Car& updatedCar)
{
    std::lock_guard<std::mutex> lock(dbMutex);

    sqlite3* db = OpenDB();

    const char* sql = R"(
			UPDATE Cars SET
				car_type = ?,
				brand = ?,
				name = ?,
				status = ?
			WHERE license_plate = ?;
		)";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare updateCar statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    // Solving lifetime issuse
    std::string type = updatedCar.carTypeToString();
    std::string status = updatedCar.carStatusToString();

    sqlite3_bind_text(stmt, 1, type.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, updatedCar.brand.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, updatedCar.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, status.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, licensePlate.c_str(), -1, SQLITE_STATIC);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    if (!success)
    {
        std::cerr << "Failed to update car: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_close(db);
    return success;
}

bool DatabaseManager::RemoveCar(const std::string& licensePlate)
{
    std::lock_guard<std::mutex> lock(dbMutex);

    sqlite3* db = OpenDB();

    const char* sql = R"(
			DELETE FROM Cars WHERE license_plate = ?;
		)";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare removeCar statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_text(stmt, 1, licensePlate.c_str(), -1, SQLITE_STATIC);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    if (!success)
    {
        std::cerr << "Failed to remove car: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_close(db);
    return success;
}

std::optional<Interfaces::Car> DatabaseManager::GetCar(const std::string& licensePlate)
{
    std::lock_guard<std::mutex> lock(dbMutex);

    sqlite3* db = OpenDB();

    const char* sql = R"(
			SELECT car_type, brand, name, status
			FROM Cars
			WHERE license_plate = ?;
		)";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare getCar statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return std::nullopt;
    }

    sqlite3_bind_text(stmt, 1, licensePlate.c_str(), -1, SQLITE_STATIC);

    std::string type;
    std::string brand;
    std::string name;
    std::string status;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        type = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        brand = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        status = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
    }
    else
    {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return std::nullopt;
    }

    sqlite3_finalize(stmt);

    sqlite3_close(db);

    return Interfaces::Car(type, licensePlate, brand, name, status);
}

std::vector<Interfaces::Car> DatabaseManager::GetAllCars()
{
    std::lock_guard<std::mutex> lock(dbMutex);

    sqlite3* db = OpenDB();

    const char* sql = R"(
			SELECT car_type, license_plate, brand, name, status
			FROM Cars;
		)";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare getAllCars statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return std::vector<Interfaces::Car>();
    }

    std::vector<Interfaces::Car> cars;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        std::string type = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        std::string licensePlate = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        std::string brand = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        std::string name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        std::string status = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));

        cars.push_back(Interfaces::Car(type, licensePlate, brand, name, status));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return cars;
}

int DatabaseManager::GetCarID(const std::string& licensePlate)
{
    std::lock_guard<std::mutex> lock(dbMutex);

    sqlite3* db = OpenDB();

    const char* sql = R"(
			SELECT id
			FROM Cars
			WHERE license_plate = ?;
		)";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare getCarID statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return -1;
    }

    sqlite3_bind_text(stmt, 1, licensePlate.c_str(), -1, SQLITE_STATIC);

    int id = -1;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        id = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return id;
}

int DatabaseManager::GetCarMilage(const std::string& licensePlate)
{
    std::lock_guard<std::mutex> lock(dbMutex);

    sqlite3* db = OpenDB();

    /*
    SELECT SUM(distance) AS mileage
FROM Trips
JOIN Cars ON Trips.car_id = Cars.id
WHERE Cars.license_plate = 'ABC123';
    */

    const char* sql = R"(
			SELECT SUM(distance) AS mileage
			FROM Trips
			JOIN Cars ON Trips.car_id = Cars.id
			WHERE Cars.license_plate = ?;
		)";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare getCarMilage statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return -1;
    }

    sqlite3_bind_text(stmt, 1, licensePlate.c_str(), -1, SQLITE_STATIC);

    int mileage = 0;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        mileage = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return mileage;
}

bool DatabaseManager::AddTrip(const int& user_id, const int& car_id, const Interfaces::Trip& trip)
{
    std::lock_guard<std::mutex> lock(dbMutex);

    sqlite3* db = OpenDB();

    const char* sql = R"(
			INSERT INTO Trips (user_id, car_id, start_circle, destination_circle, distance, cost, start_rental_date, end_rental_date)
			VALUES (?, ?, ?, ?, ?, ?, ?, ?);
)";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare addTrip statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Solving lifetime issuse
    std::string startCircle = trip.startCircleToString();
    std::string destinationCircle = trip.destinationCircleToString();

    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, car_id);
    sqlite3_bind_text(stmt, 3, startCircle.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, destinationCircle.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, trip.distance);
    sqlite3_bind_double(stmt, 6, trip.cost);
    sqlite3_bind_text(stmt, 7, trip.start_rental_date.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 8, trip.end_rental_date.c_str(), -1, SQLITE_STATIC);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    if (!success)
    {
        std::cerr << "Failed to add trip: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_close(db);
    return success;
}

/*
struct TripInfo
{
    Car car;
    User user;
    Trip trip;
};
*/
std::vector<Interfaces::TripInfo> DatabaseManager::GetUserTrips(const std::string& drivingLicense)
{
    std::lock_guard<std::mutex> lock(dbMutex);

    sqlite3* db = OpenDB();

    // Get car, user and trip information for a given driving license
    const char* sql = R"(
			SELECT Cars.car_type, Cars.license_plate, Cars.brand, Cars.name, Cars.status,
				Users.name, Users.surname, Users.address, Users.credit_card, Users.driving_license,
				Trips.start_circle, Trips.destination_circle, Trips.distance, Trips.cost, Trips.start_rental_date, Trips.end_rental_date
			FROM Trips
			JOIN Cars ON Trips.car_id = Cars.id
			JOIN Users ON Trips.user_id = Users.id
			WHERE Users.driving_license = ?;
		)";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare getUserTrips statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return std::vector<Interfaces::TripInfo>();
    }

    sqlite3_bind_text(stmt, 1, drivingLicense.c_str(), -1, SQLITE_STATIC);

    std::vector<Interfaces::TripInfo> trips;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        std::string carType = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        std::string licensePlate = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        std::string brand = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        std::string name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        std::string status = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));

		std::string userName = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));
		std::string userSurname = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6)));
		std::string userAddress = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7)));
		std::string creditCard = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8)));
		std::string userDrivingLicense = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9)));

		std::string startCircle = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10)));
		std::string destinationCircle = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 11)));
		int distance = sqlite3_column_int(stmt, 12);
		double cost = sqlite3_column_double(stmt, 13);
		std::string startRentalDate = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 14)));
		std::string endRentalDate = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 15)));

        Interfaces::User user(userName, userSurname, userAddress, creditCard, userDrivingLicense);
        Interfaces::Car car(carType, licensePlate, brand, name, status);
        Interfaces::Trip trip(startCircle, destinationCircle, distance, cost, startRentalDate, endRentalDate);
        Interfaces::TripInfo tripInfo = {car=car, user=user, trip=trip};
        trips.push_back(tripInfo);		
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return trips;
}

bool DatabaseManager::AddService(const std::string& licensePlate)
{
    // Add a service for the car with the given license plate. I cannot use GetCarID for deadlock reasons

    sqlite3* db = OpenDB();

    const char* sql = R"(
			SELECT date
			FROM CurrentDate
			WHERE id = 1;
		)";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare getCurrentDate statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    std::string date;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        date = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    }

    sqlite3_finalize(stmt);


    const char* sql2 = R"(
			INSERT INTO Services (car_id, service_date, distance_since_last_service)
			VALUES ((SELECT id FROM Cars WHERE license_plate = ?), ?, 0);
		)";

    sqlite3_stmt* stmt2;
    if (sqlite3_prepare_v2(db, sql2, -1, &stmt2, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare addService statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt2, 1, licensePlate.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt2, 2, date.c_str(), -1, SQLITE_STATIC);

    bool success = (sqlite3_step(stmt2) == SQLITE_DONE);
    sqlite3_finalize(stmt2);

    if (!success)
    {
        std::cerr << "Failed to add service: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_close(db);
    return success;
    
}

std::string DatabaseManager::GetCurrentDate(bool lockDb)
{
    if (lockDb)
    {
        std::lock_guard<std::mutex> lock(dbMutex);
        return GetCurrentDate(false);
    }

    sqlite3* db = OpenDB();

    const char* sql = R"(
			SELECT date
			FROM CurrentDate
			WHERE id = 1;
		)";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare getCurrentDate statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return "";
    }

    std::string date;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        date = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return date;
}


bool DatabaseManager::UpdateDatabase()
{

    if (!ManageEndOfRentals())
    {
        return false;
    }

    if (!UpdateDistanceSinceLastService())
    {
        return false;
    }

    if (!CheckServiceCompleted())
    {
        return false;
    }

    if (!CheckNeedOfService())
    {
        return false;
    }

    return true;
}

bool DatabaseManager::CheckServiceCompleted()
{
    // If today is equal to service_date + 1 day and car is under service, we need to update 
    // the status of the car to available
    auto currentDate = GetCurrentDate(false);

    sqlite3* db = OpenDB();

    const char* sql = R"(
			SELECT Cars.id, Cars.license_plate, Services.service_date
			FROM Cars
			JOIN Services ON Cars.id = Services.car_id
			WHERE Services.service_date = date(?, '-1 day') AND Cars.status = 'under_service';
		)";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare CheckServiceCompleted statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_text(stmt, 1, currentDate.c_str(), -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int carID = sqlite3_column_int(stmt, 0);
        std::string licensePlate = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));

        const char* sql2 = R"(
				UPDATE Cars
				SET status = 'available'
				WHERE id = ?;
			)";

        sqlite3_stmt* stmt2;
        if (sqlite3_prepare_v2(db, sql2, -1, &stmt2, nullptr) != SQLITE_OK)
        {
            std::cerr << "Failed to prepare CheckServiceCompleted statement2: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return false;
        }

        sqlite3_bind_int(stmt2, 1, carID);

        if (sqlite3_step(stmt2) != SQLITE_DONE)
        {
            std::cerr << "Failed to update car status: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }

        sqlite3_finalize(stmt2);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return true;
}

bool DatabaseManager::CheckNeedOfService()
{
    // If distance_since_last_service >= MAX_DISTANCE, we need to update service_date to today and
    // distance_since_last_service to 0. Also we need to update the status of the car to under_service

    auto currentDate = GetCurrentDate(false);

    sqlite3* db = OpenDB();

    const char* sql = R"(
			SELECT Cars.id, Cars.license_plate, Services.distance_since_last_service
			FROM Cars
			JOIN Services ON Cars.id = Services.car_id
			WHERE Services.distance_since_last_service >= ?;
		)";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare CheckNeedOfService statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_int(stmt, 1, MAX_DISTANCE);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int carID = sqlite3_column_int(stmt, 0);
        std::string licensePlate = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));

		const char* sql2 = R"(
				UPDATE Services
				SET service_date = ?, distance_since_last_service = 0
				WHERE car_id = ?;
			)";

		sqlite3_stmt* stmt2;
		if (sqlite3_prepare_v2(db, sql2, -1, &stmt2, nullptr) != SQLITE_OK)
		{
            std::cerr << "Failed to prepare CheckNeedOfService statement2: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return false;
		}

        sqlite3_bind_text(stmt2, 1, currentDate.c_str(), -1, SQLITE_STATIC);
		sqlite3_bind_int(stmt2, 2, carID);

		if (sqlite3_step(stmt2) != SQLITE_DONE)
		{
            std::cerr << "Failed to update service date and distance since last service: " << sqlite3_errmsg(db)
                      << std::endl;
		}

		sqlite3_finalize(stmt2);

		const char* sql3 = R"(
				UPDATE Cars
				SET status = 'under_service'
				WHERE id = ?;
			)";

		sqlite3_stmt* stmt3;
		if (sqlite3_prepare_v2(db, sql3, -1, &stmt3, nullptr) != SQLITE_OK)
		{
            std::cerr << "Failed to prepare CheckNeedOfService statement3: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return false;
		}

		sqlite3_bind_int(stmt3, 1, carID);

		if (sqlite3_step(stmt3) != SQLITE_DONE)
		{
            std::cerr << "Failed to update car status: " << sqlite3_errmsg(db) << std::endl;
		}

		sqlite3_finalize(stmt3);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return true;
}

bool DatabaseManager::UpdateDistanceSinceLastService()
{
    // If current_day is equal to end_rental_date + 1 day, we need to update the distance_since_last_service based on the trip distance
    auto currentDate = GetCurrentDate(false);

    sqlite3* db = OpenDB();

    const char* sql = R"(
			SELECT Trips.id, Trips.car_id, Trips.distance
			FROM Trips
			WHERE end_rental_date = date(?, '-1 day');
		)";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare UpdateDistanceSinceLastService statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_text(stmt, 1, currentDate.c_str(), -1, SQLITE_STATIC);


    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int tripID = sqlite3_column_int(stmt, 0);
        int carID = sqlite3_column_int(stmt, 1);
        int distance = sqlite3_column_int(stmt, 2);

        const char* sql2 = R"(
				UPDATE Services
				SET distance_since_last_service = distance_since_last_service + ?
				WHERE car_id = ?;
			)";

        sqlite3_stmt* stmt2;
        if (sqlite3_prepare_v2(db, sql2, -1, &stmt2, nullptr) != SQLITE_OK)
        {
            std::cerr << "Failed to prepare UpdateDistanceSinceLastService statement2: " << sqlite3_errmsg(db)
                      << std::endl;
            sqlite3_close(db);
            return false;
        }

        sqlite3_bind_int(stmt2, 1, distance);
        sqlite3_bind_int(stmt2, 2, carID);

        if (sqlite3_step(stmt2) != SQLITE_DONE)
        {
            std::cerr << "Failed to update distance since last service: " << sqlite3_errmsg(db) << std::endl;
        }

        sqlite3_finalize(stmt2);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return true;
}

bool DatabaseManager::ManageEndOfRentals()
{
    // If current_day is equal to end_rental_date + 1 day, we need to update the status of the car to available    
    auto currentDate = GetCurrentDate(false);

    sqlite3* db = OpenDB();

    const char* sql = R"(
			SELECT id, car_id
			FROM Trips
			WHERE end_rental_date = date(?, '-1 day');
		)";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare ManageEndOfRentals statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_text(stmt, 1, currentDate.c_str(), -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int tripID = sqlite3_column_int(stmt, 0);
        int carID = sqlite3_column_int(stmt, 1);

        const char* sql2 = R"(
				UPDATE Cars
				SET status = 'available'
				WHERE id = ?;
			)";

        sqlite3_stmt* stmt2;
        if (sqlite3_prepare_v2(db, sql2, -1, &stmt2, nullptr) != SQLITE_OK)
        {
            std::cerr << "Failed to prepare ManageEndOfRentals statement2: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return false;
        }

        sqlite3_bind_int(stmt2, 1, carID);

        if (sqlite3_step(stmt2) != SQLITE_DONE)
        {
            std::cerr << "Failed to update car status: " << sqlite3_errmsg(db) << std::endl;
        }

        sqlite3_finalize(stmt2);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return true;
}

void DatabaseManager::CreateTables(sqlite3* db)
{
    // TODO: CHECK (type IN ('ECO', 'MID-CLASS', 'DELUXE'))
    // TODO:  CHECK (status IN ('available', 'rented', 'under_service'))
    const char* createCarsTable = R"(
        CREATE TABLE IF NOT EXISTS Cars (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            car_type TEXT NOT NULL CHECK (car_type IN ('ECO', 'MID-CLASS', 'DELUXE')),
            license_plate TEXT UNIQUE NOT NULL,
            brand TEXT NOT NULL,
            name TEXT NOT NULL,
            status TEXT NOT NULL CHECK (status IN ('available', 'rented', 'under_service'))
        );
    )";

    const char* createUsersTable = R"(
        CREATE TABLE IF NOT EXISTS Users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            surname TEXT NOT NULL,
            address TEXT NOT NULL,
            credit_card TEXT NOT NULL,
            driving_license TEXT UNIQUE NOT NULL
        );
    )";

    const char* createTripsTable = R"(
        CREATE TABLE IF NOT EXISTS Trips (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            car_id INTEGER NOT NULL,
            start_circle TEXT NOT NULL CHECK (start_circle IN ('Inner Circle', 'Middle Circle', 'Outer Circle')),
            destination_circle TEXT NOT NULL CHECK (destination_circle IN ('Inner Circle', 'Middle Circle', 'Outer Circle')),
            distance INTEGER NOT NULL,
            cost REAL NOT NULL,
            start_rental_date DATE NOT NULL,
            end_rental_date DATE NOT NULL,
            FOREIGN KEY (user_id) REFERENCES Users(id),
            FOREIGN KEY (car_id) REFERENCES Cars(id)
        );
    )";

    const char* createServicesTable = R"(
        CREATE TABLE IF NOT EXISTS Services (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            car_id INTEGER NOT NULL,
            service_date DATE NOT NULL,
            distance_since_last_service INTEGER NOT NULL,
            FOREIGN KEY (car_id) REFERENCES Cars(id)
        );
    )";

    const char* createCurrentDateTable = R"(
        CREATE TABLE IF NOT EXISTS CurrentDate (
            id INTEGER PRIMARY KEY DEFAULT 1,
            date DATE NOT NULL
        );
    )";

    char* errorMessage;

    // Create Cars table
    if (sqlite3_exec(db, createCarsTable, nullptr, nullptr, &errorMessage) != SQLITE_OK)
    {
        std::cerr << "Error creating Cars table: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }

    // Create Users table
    if (sqlite3_exec(db, createUsersTable, nullptr, nullptr, &errorMessage) != SQLITE_OK)
    {
        std::cerr << "Error creating Users table: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }

    // Create Trips table
    if (sqlite3_exec(db, createTripsTable, nullptr, nullptr, &errorMessage) != SQLITE_OK)
    {
        std::cerr << "Error creating Trips table: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }

    // Create Services table
    if (sqlite3_exec(db, createServicesTable, nullptr, nullptr, &errorMessage) != SQLITE_OK)
    {
        std::cerr << "Error creating Services table: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }

    // Create CurrentDate table
    if (sqlite3_exec(db, createCurrentDateTable, nullptr, nullptr, &errorMessage) != SQLITE_OK)
    {
        std::cerr << "Error creating CurrentDate table: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }

    // Insert initial date if not exists
    const char* insertDate = R"(
		INSERT OR IGNORE INTO CurrentDate (date)
		VALUES (?);
	)";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, insertDate, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare insertDate statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, INITIAL_DATE.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        std::cerr << "Failed to insert initial date" << std::endl;
    }

    sqlite3_finalize(stmt);
}

bool DatabaseManager::NextDay()
{
    std::lock_guard<std::mutex> lock(dbMutex);

    sqlite3* db = OpenDB();

    const char* sql = R"(
			UPDATE CurrentDate
			SET date = date(date, '+1 day')
			WHERE id = 1;
		)";

    char* errorMessage;
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errorMessage) != SQLITE_OK)
    {
        std::cerr << "Failed to update date: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        sqlite3_close(db);
        return false;
    }

    sqlite3_close(db);

    return UpdateDatabase();
}

std::optional<Interfaces::Service> DatabaseManager::GetService(const std::string& licensePlate)
{
    std::lock_guard<std::mutex> lock(dbMutex);

    sqlite3* db = OpenDB();

    const char* sql = R"(
			SELECT service_date, distance_since_last_service
			FROM Services
			JOIN Cars ON Services.car_id = Cars.id
			WHERE Cars.license_plate = ?;
		)";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare getService statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return std::nullopt;
    }

    sqlite3_bind_text(stmt, 1, licensePlate.c_str(), -1, SQLITE_STATIC);

    std::string serviceDate;
    int distanceSinceLastService;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        serviceDate = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        distanceSinceLastService = sqlite3_column_int(stmt, 1);
    }
    else
    {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return std::nullopt;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return Interfaces::Service(serviceDate, distanceSinceLastService);

}

std::string DatabaseManager::GetNextDate(int numDays)
{
    std::lock_guard<std::mutex> lock(dbMutex);

    sqlite3* db = OpenDB();

    const char* sql = R"(
			SELECT date(date, '+' || ? || ' day')
			FROM CurrentDate
			WHERE id = 1;
		)";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare getNextDate statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return "";
    }

    sqlite3_bind_int(stmt, 1, numDays);

    std::string date;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        date = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return date;
}