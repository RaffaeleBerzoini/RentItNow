add_library (Common STATIC
	include/StructureInterface.h
	include/CoutSilencer.h
	src/CoutSilencer.cpp
	)

target_include_directories (Common PUBLIC
	include
)

