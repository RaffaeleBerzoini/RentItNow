#include "UserManager.h"
#include "CarManager.h"

class Core {
private:
    std::unique_ptr<UserManager> userManager;
    std::unique_ptr<CarManager> carManager;
    std::shared_ptr<DatabaseManager> databaseManager;
    Core();

    inline static Core* instance = nullptr;

public:

    static Core* GetInstance();

    UserManager& GetUserManager();
    CarManager& GetCarManager();
};