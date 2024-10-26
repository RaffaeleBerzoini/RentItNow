cmake_minimum_required(VERSION 3.10)
project(RentItNow)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)

include(".cmake/functions.cmake")

# Set the output directory for all the executables and DLLs to the same folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/)

add_subdirectory("Code")

