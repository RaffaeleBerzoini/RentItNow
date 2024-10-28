#include "Interface.h"
#include <string>

using namespace API::Server;

// singleton
class Server {
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
        API::Server::NextDay();
    }

    std::string GetCurrentDate()
    {
        return API::Server::GetCurrentDate();
    }

};