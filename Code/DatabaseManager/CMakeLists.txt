add_library(DatabaseManager SHARED 
    include/DatabaseManager.h
    src/DatabaseManager.cpp
)

target_include_directories(DatabaseManager PUBLIC
    include
)

target_link_libraries(DatabaseManager PRIVATE
    sqlite3
)