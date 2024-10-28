# Setup

```shell
git clone https://github.com/RaffaeleBerzoini/RentItNow
cd RentItNow
mkdir build
cd build
cmake ..
cmake --build . --config Debug
cmake --build . --config Release
```

# Run

```shell
cd .\bin\Release
.\RentItNow.exe
.\UnitTests.exe
```

Or download the release from the [release page](https://github.com/RaffaeleBerzoini/RentItNow/releases)

# Some notes

![RentItNow Architecture](Diagrams/RentItNowArchitecture.png)

## RentItNow car sharing
 
You are software engineer at **RentItNow**, a car sharing company located in SimpleTown.
SimpleTown is a rounded village dived in three concentric circles: **Inner Circle**, **Middle Circle**, **Outer Circle**.
 
Your boss asks you to develop a new software to manage company's cars and users.
The boss needs to:
 
- Check the status of the car: location, total distance traveled, next service time, availability.
 
Optional actions:
- Add, update and remove cars;
- Add, update and remove users;
 
A car has (at least)
- a type,
- a license plate,
- a brand,
- a name
 
There are three types of car:
- **ECO**, max 2 persons,
- **MID-CLASS**, max 4 persons,
- **DELUXE**, max 7 persons.
 
Each type of car has a rental price per km:
- **ECO**: 1$/km;
- **MID-CLASS**: 2$/km;
- **DELUXE**: 5$/km
 
Each type of car has a fixed speed:
- **ECO**: 15km/h
- **MID-CLASS**: 25km/h
- **DELUXE**: 50km/h
 
The company must **service** its cars every 1500km. Service takes 1 day and cost 300$. Car cannot be rented on that day.
 
A **user** can register to the company service, update its data and delete its account. User has (at least):
- name,
- surname,
- address,
- credit card,
- driving license
 
A user can ask for a car: select type of car, number of passengers, starting circle and destination circle.
 
Travel distance is computed based on **hops**; an hop is 5km; an hops is going from one circle to the next one. **Always counts 1 hop for the starting circle**
(e.g. travelling from Inner Circle to Middle Circle is 2 hops, not 1 hop). Travelling in the same circle is 1 hop.
 
The software select the best car for the user based on some metric (up to you), calculate the cost of the trip and make the payment.
If no car is available, the software presents the user an expected waiting time.
 
 
### Preferred language:
- C++ with Qt framework
 
### Extra points:
- Create a git repository on a gitlab server;
- Setup the project for that repository;
- Commit and push the code there.
- use CMake
 
### Nice to have (considered as a plus)
- Package Manager (vcpkg, conan, ....)
- CI/CD  pipeline (Gilab or GitHub Actions), also a draft is ok
- Deployed executable
### Tools
- You can use any IDE you like
- Feel free to browse internet for help or drop us an email
