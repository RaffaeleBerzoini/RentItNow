#include "Core.h"

#ifdef INTERFACE_EXPORT
#    define API_INTERFACE_EXPORT __declspec(dllexport)
#else
#    define API_INTERFACE_EXPORT __declspec(dllimport)
#endif

using CORE = Core* (*)();
CORE CorePtr = Core::GetInstance;

namespace API
{
	namespace Common
	{
        API_INTERFACE_EXPORT bool AddUser(const Interfaces::User& user);
	}
	namespace Boss
	{

	}

	namespace User
    {
	
	}

}