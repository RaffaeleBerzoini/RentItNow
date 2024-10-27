#include <vector>
#include <optional>

#ifdef INTERFACE_EXPORT
#    define API_INTERFACE_EXPORT __declspec(dllexport)
#else
#    define API_INTERFACE_EXPORT __declspec(dllimport)
#endif

namespace API
{
	namespace Common
	{
        API_INTERFACE_EXPORT bool AddUser(const Interfaces::User& user);
        API_INTERFACE_EXPORT bool UpdateUser(const std::string& driving_license, const Interfaces::User& user);
        API_INTERFACE_EXPORT bool RemoveUser(const std::string& drivingLicense);
        API_INTERFACE_EXPORT std::optional<Interfaces::User> GetUser(const std::string& drivingLicense);
        API_INTERFACE_EXPORT std::vector<Interfaces::User> GetAllUsers();
	}
	namespace Boss
	{

	}

	namespace User
    {
	
	}

}