cmake_minimum_required(VERSION 3.10)
project(RentItNow)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)

include(".cmake/functions.cmake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory("Code")

