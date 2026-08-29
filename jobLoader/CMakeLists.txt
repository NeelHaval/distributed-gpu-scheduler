add_library(jobLoader
    src/JobLoader.cpp
)

target_include_directories(jobLoader
    PUBLIC
    include
)

target_link_libraries(jobLoader
    PUBLIC
    common
)