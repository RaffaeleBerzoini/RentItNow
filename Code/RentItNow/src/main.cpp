#include <iostream>
#include "StructureInterface.h"
#include "Server.h"
#include "Boss.h"
#include "User.h"

using SERVER = Server* (*)();
SERVER ServerPtr = Server::GetInstance;

using BOSS = Boss* (*)();
BOSS BossPtr = Boss::GetInstance;

int main()
{
    std::cout << "Current date: " << ServerPtr()->GetCurrentDate() << std::endl;
    ServerPtr()->NextDay();
    std::cout << "Current date: " << ServerPtr()->GetCurrentDate() << std::endl;
    return 0;
}
