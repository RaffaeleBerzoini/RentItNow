add_library (Common STATIC
	include/StructureInterface.h
	src/neededForLib.cpp
	)

target_include_directories (Common PUBLIC
	include
)

