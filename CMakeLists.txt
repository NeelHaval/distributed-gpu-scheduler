cmake_minimum_required(VERSION 3.20)

project(DistributedGPUScheduler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(common)
add_subdirectory(worker)
add_subdirectory(scheduler)
add_subdirectory(external/googletest)
add_subdirectory(tests)

# NEW
add_executable(gpu_scheduler
    scheduler/src/main.cpp
)

# Link against your libraries
target_link_libraries(gpu_scheduler
    PRIVATE
    scheduler
    worker
    common
)

add_executable(client_app 
        client/src/main.cpp)

target_link_libraries(client_app 
        PRIVATE 
        common 
        worker 
        scheduler)

add_executable(worker_app 
        worker/src/main.cpp)
    
target_link_libraries(worker_app 
        PRIVATE 
        common
        worker 
        scheduler)

# Include directories for main executable
target_include_directories(gpu_scheduler
    PRIVATE
    scheduler/include
    worker/include
    common/include
)