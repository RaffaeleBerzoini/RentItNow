add_executable (UnitTests
  src/UnitTests.cpp
  src/DatabaseManager/DatabaseManagerTest.cpp
  src/UserManager/UserManagerTest.cpp
  src/BookingManager/BookingManagerTest.cpp
  src/Core/CoreTest.cpp
	)

target_link_libraries (UnitTests PRIVATE
  Catch2::Catch2WithMain
  Core
)