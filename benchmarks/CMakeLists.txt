add_library(benchmarks
    src/Benchmark.cpp
)

target_include_directories(benchmarks
    PUBLIC
    include
)