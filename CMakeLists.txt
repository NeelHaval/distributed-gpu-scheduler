cmake_minimum_required(VERSION 3.20)

project(DistributedGPUScheduler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(common)
add_subdirectory(worker)
add_subdirectory(scheduler)
add_subdirectory(external/googletest)
add_subdirectory(tests)