#include "Interface.h"
#include <string>
#include "CoutSilencer.h"

using namespace API::Server;

// singleton
class Server : private CoutSilencer {
private:
    Server() = default;

    inline static Server* instance = nullptr;    

public:

    inline static Server* GetInstance()
    {
        static Server instance;
        return &instance;
    }

    void NextDay()
    {
        SilenceCout();
        API::Server::NextDay();
        RestoreCout();
    }

    std::string GetCurrentDate()
    {
        return API::Server::GetCurrentDate();
    }

};