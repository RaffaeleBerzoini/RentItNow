#include "DatabaseManager.h"

class UserManager
{
private:
    std::shared_ptr<DatabaseManager> dbManager;

public:
	bool AddUser(const User& user);
    bool UpdateUser(const std::string& driving_license, const User& user);
    bool RemoveUser(const std::string& drivingLicense);
    std::optional<User> GetUser(const std::string& drivingLicense);

    UserManager(std::shared_ptr<DatabaseManager> dbManagerPtr);
};