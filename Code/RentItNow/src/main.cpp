#include <iostream>
#include "StructureInterface.h"
#include "Interface.h"

int main()
{
    // TODO: threads/classes for boss and user consoles

    // Boss logic

    Interfaces::User user("Raffa", "Berzo", "123 Main St", "1234 5678 9012 3456", "AA0011");
    API::Common::AddUser(user);

    user.address = "456 Main St";
    API::Common::AddUser(user);

    Interfaces::User user2("Mary", "Jane", "789 Main St", "1234 5678 9999 3456", "AA0012");
    API::Common::AddUser(user2);

    auto users = API::Common::GetAllUsers();

    for (const auto& user : users)
    {
        std::cout << user.name << " " << user.surname << " " << user.address << " " << user.drivingLicense << " "
                  << user.creditCard << std::endl;
    }

    // Modify user
    user.creditCard = "6543 2109 8765 4321";
    API::Common::UpdateUser("AA0011", user);

    // Remove user
    API::Common::RemoveUser("AA0012");

    users = API::Common::GetAllUsers();

    for (const auto& user : users)
    {
        std::cout << user.name << " " << user.surname << " " << user.address << " " << user.drivingLicense << " "
                  << user.creditCard << std::endl;
    }

    return 0;
}
