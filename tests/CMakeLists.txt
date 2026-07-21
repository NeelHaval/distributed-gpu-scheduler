add_executable(runTests
    JobTests.cpp
    WorkerTests.cpp
    SchedulerTests.cpp
)

target_link_libraries(runTests
    PRIVATE
    gtest
    gtest_main
    common
    worker
    scheduler
)

include(GoogleTest)
gtest_discover_tests(runTests)