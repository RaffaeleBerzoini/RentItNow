# Create the executable
add_executable(RenItNow 
    src/main.cpp
)

# Link the executable to the SQLite3 library
target_link_libraries(RenItNow sqlite3 API)