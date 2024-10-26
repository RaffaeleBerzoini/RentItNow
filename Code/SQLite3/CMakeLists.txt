add_library(sqlite3 STATIC 
include/sqlite3.h
src/sqlite3.c)

target_include_directories(sqlite3 PUBLIC include/)