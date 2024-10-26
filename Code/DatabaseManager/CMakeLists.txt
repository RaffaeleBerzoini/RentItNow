add_library(DatabaseManager SHARED 
    include/DatabaseManager.h
    src/DatabaseManager.cpp
)

add_compile_definitions(DBMANAGER_EXPORT)

target_include_directories(DatabaseManager PUBLIC
    include
)

target_link_libraries(DatabaseManager PUBLIC
    sqlite3
)