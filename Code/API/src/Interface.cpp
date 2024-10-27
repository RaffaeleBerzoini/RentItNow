#include "Core.h"
#include "Interface.h"

using namespace API;

using CORE = Core* (*)();
CORE CorePtr = Core::GetInstance;

bool Common::AddUser(const Interfaces::User& user)
{
    return CorePtr()->GetUserManager().AddUser(user);
}

bool Common::UpdateUser(const std::string& driving_license, const Interfaces::User& user)
{
    return CorePtr()->GetUserManager().UpdateUser(driving_license, user);
}

bool Common::RemoveUser(const std::string& drivingLicense)
{
    return CorePtr()->GetUserManager().RemoveUser(drivingLicense);
}

std::optional<Interfaces::User> Common::GetUser(const std::string& drivingLicense)
{
    return CorePtr()->GetUserManager().GetUser(drivingLicense);
}

//std::vector<Interfaces::User> Common::GetAllUsers()
//{
//    return CorePtr()->GetUserManager().GetAllUsers();
//}