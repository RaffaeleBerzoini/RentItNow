#include "UserManager.h"
#include "CarManager.h"
#include "BookingManager.h"

class Core {
private:
    std::unique_ptr<UserManager> userManager;
    std::unique_ptr<CarManager> carManager;
    std::shared_ptr<DatabaseManager> databaseManager;
    std::unique_ptr<BookingManager> bookingManager;
    Core();

    inline static Core* instance = nullptr;
    inline static std::string DATABASE_PATH = "database/database.db";

public:

    static Core* GetInstance();
    static void SetDatabasePath(const std::string& path)
    {
        DATABASE_PATH = path;
    }

    UserManager& GetUserManager();
    CarManager& GetCarManager();
    BookingManager& GetBookingManager();

    // Utility functions from DatabaseManager. I do not want to expose the DatabaseManager to the API
    void NextDay();
    std::string GetCurrentDate();
};