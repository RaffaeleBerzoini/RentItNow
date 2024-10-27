#include "DatabaseManager.h"

class UserManager
{
private:
    std::shared_ptr<DatabaseManager> dbManager;

public:
	void AddUser(const User& user);
    void UpdateUser(const std::string& driving_license, const User& user);
    void RemoveUser(const std::string& drivingLicense);
    std::optional<User> GetUser(const std::string& drivingLicense);

    UserManager(std::shared_ptr<DatabaseManager> dbManagerPtr);
};