#include <iostream>
#include "StructureInterface.h"
#include "Server.h"

using SERVER = Server* (*)();
SERVER ServerPtr = Server::GetInstance;

int main()
{
    std::cout << "Current date: " << ServerPtr()->GetCurrentDate() << std::endl;
    ServerPtr()->NextDay();
    std::cout << "Current date: " << ServerPtr()->GetCurrentDate() << std::endl;
    return 0;
}
