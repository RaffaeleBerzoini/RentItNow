add_library(CarManager STATIC 
    include/CarManager.h
    src/CarManager.cpp
)

target_include_directories(CarManager PUBLIC
    include
)

target_link_libraries(CarManager PUBLIC
    DatabaseManager    
)