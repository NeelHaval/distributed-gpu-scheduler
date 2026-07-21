add_library(scheduler
    src/Scheduler.cpp
)

target_include_directories(scheduler PUBLIC include)

target_link_libraries(scheduler
    PUBLIC
    common
    worker
)