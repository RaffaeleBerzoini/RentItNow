#include "DatabaseManager.h"

class UserManager
{
private:
    std::shared_ptr<DatabaseManager> dbManager;

public:
    bool AddUser(const Interfaces::User& user);
    bool UpdateUser(const std::string& driving_license, const Interfaces::User& user);
    bool RemoveUser(const std::string& drivingLicense);
    std::optional<Interfaces::User> GetUser(const std::string& drivingLicense);

    UserManager(std::shared_ptr<DatabaseManager> dbManagerPtr);
};