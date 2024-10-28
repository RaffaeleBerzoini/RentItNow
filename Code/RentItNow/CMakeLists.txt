# Create the executable
add_executable(RenItNow 
    src/main.cpp
    include/Boss.h
    src/Boss.cpp
    include/User.h
    src/User.cpp
    include/Server.h
)

target_include_directories(RenItNow PRIVATE include)

# Link the executable to the SQLite3 library
target_link_libraries(RenItNow sqlite3 API)