add_library (API SHARED
	include/Interface.h		
	src/Interface.cpp
)

add_compile_definitions(INTERFACE_EXPORT)

target_include_directories (API PUBLIC
    include
)

target_link_libraries (API PUBLIC
    Core
)

