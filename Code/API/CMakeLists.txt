add_library (API SHARED
	include/Interface.h		
	src/Interface.cpp
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET API PROPERTY CXX_STANDARD 20)
endif()

add_compile_definitions(INTERFACE_EXPORT)

target_include_directories (API PRIVATE
    src
)

target_include_directories (API PUBLIC
    include
)

target_link_libraries (API PUBLIC
    Fuze_Core
    Wire_Log
)

