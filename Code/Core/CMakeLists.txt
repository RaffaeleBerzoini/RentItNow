add_library (Core STATIC
    include/Core.h
    src/Core.cpp
	)

target_include_directories (Core PUBLIC
	include
)

target_link_libraries (Core PUBLIC
    Common
)


