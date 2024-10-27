add_library(UserManager STATIC 
    include/UserManager.h
    src/UserManager.cpp
)

target_include_directories(UserManager PUBLIC
    include
)

target_link_libraries(UserManager PUBLIC
    DatabaseManager    
)