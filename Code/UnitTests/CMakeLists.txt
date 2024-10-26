add_executable (UnitTests
  src/UnitTests.cpp
  src/DatabaseManager/DatabaseManagerTest.cpp
	)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET UnitTests PROPERTY CXX_STANDARD 20)
endif()

make_source_group(TARGET UnitTests)

target_link_libraries (UnitTests PRIVATE
  Catch2::Catch2WithMain
)

target_link_libraries(UnitTests PUBLIC
  DatabaseManager
)