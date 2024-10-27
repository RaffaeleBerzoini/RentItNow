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
//API::Common::AddUser(user);

    return 0;
}
