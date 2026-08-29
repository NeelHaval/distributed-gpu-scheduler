#include "Benchmark.h"
#include <iostream>
#include <iomanip>

// Constructor for benchmark
Benchmark::Benchmark(int expectedWorkers)

    : expectedWorkers(expectedWorkers)

    {

    }

// Mark start time
void Benchmark::start() {

    startTime = std::chrono::steady_clock::now();

}

// Mark stop time
void Benchmark::stop() {

    endTime = std::chrono::steady_clock::now();

}

// Calculate elapsed time
double Benchmark::getElapsedMilliseconds() const {

    return std::chrono::duration<double, std::milli>(endTime - startTime).count();

}

// Print result to terminal
void Benchmark::printResult(const std::string& workloadName) const {

    std::cout << "\n========== BENCHMARK ==========\n";
    std::cout << "Workload: " << workloadName << "\n";
    std::cout << "Completion time: "
              << std::fixed
              << std::setprecision(2)
              << getElapsedMilliseconds()
              << " ms\n";
    std::cout << "================================\n";

}

// Getter for expected workers
int Benchmark::getExpectedWorkers() const {

    return expectedWorkers;

}