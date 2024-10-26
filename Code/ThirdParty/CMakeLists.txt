add_subdirectory("SQLite3")

# ----- Catch2 -----
if (NOT TARGET Catch2)
    message(STATUS "Adding Catch2")
    update_submodule("Catch2" "--recursive")
    add_subdirectory(Catch2)
endif()