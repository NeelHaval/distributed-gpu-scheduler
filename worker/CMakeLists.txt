add_library(worker
    src/Worker.cpp
)

target_include_directories(worker PUBLIC include)

target_link_libraries(worker PUBLIC common)