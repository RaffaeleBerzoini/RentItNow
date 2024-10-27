add_executable (UnitTests
  src/UnitTests.cpp
  src/DatabaseManager/DatabaseManagerTest.cpp
	)

target_link_libraries (UnitTests PRIVATE
  Catch2::Catch2WithMain
  DatabaseManager
)