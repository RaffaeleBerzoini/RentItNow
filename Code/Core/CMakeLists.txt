add_library (Core STATIC
    include/Core.h
    src/Core.cpp
	)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Core PROPERTY CXX_STANDARD 20)
endif()

target_include_directories (Core PUBLIC
	include
)

target_link_libraries (Core PUBLIC
    Common
)


