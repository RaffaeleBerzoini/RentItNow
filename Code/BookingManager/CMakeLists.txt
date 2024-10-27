add_library(BookingManager STATIC 
    include/BookingManager.h
    src/BookingManager.cpp
)

target_include_directories(BookingManager PUBLIC
    include
)

target_link_libraries(BookingManager PUBLIC
    DatabaseManager    
)