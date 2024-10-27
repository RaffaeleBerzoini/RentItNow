#include "UserManager.h"

class Core {
private:
    std::unique_ptr<UserManager> userManager;
    std::shared_ptr<DatabaseManager> databaseManager;
    Core();

    inline static Core* instance = nullptr;

public:

    static Core* GetInstance();

    UserManager& GetUserManager();
};