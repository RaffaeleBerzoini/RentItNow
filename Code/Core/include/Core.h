#include "UserManager.h"

class Core {
private:
    UserManager userManager;
    std::shared_ptr<DatabaseManager> databaseManager;
    Core();

    inline static Core* instance = nullptr;

public:

    static Core* GetInstance();

    UserManager& GetUserManager();
};