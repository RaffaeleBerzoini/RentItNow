# Create the executable
add_executable(User 
    src/main.cpp
)

# Link the executable to the SQLite3 library
target_link_libraries(User sqlite3)