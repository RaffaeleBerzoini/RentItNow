cmake_minimum_required(VERSION 3.10)
project(SQLiteExample)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 11)

add_library(sqlite3 STATIC 
sqlite3.h
sqlite3.c)

# Include the SQLite3 library
target_include_directories(sqlite3 PUBLIC .)

# Create the executable
add_executable(SQLiteExample main.cpp)

# Link the executable to the SQLite3 library
target_link_libraries(SQLiteExample sqlite3)



