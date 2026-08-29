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

// Calculate the job execution time
void Benchmark::recordJobTime(const std::string& jobID, double durationMs) {

    jobTimes.push_back(durationMs);

}

// Get the throughput
double Benchmark::getThroughput(int completedJobs) const {

    double elapsedSeconds = getElapsedMilliseconds() / 1000.0;

    if (elapsedSeconds <= 0.0) {

        return 0.0;

    }

    // Throughput is jobs per second
    return completedJobs / elapsedSeconds;

}

// Calculate average job execution metric
double Benchmark::getAverageJobTime() const {

    // If no jobs were submitted/completed
    if (jobTimes.empty()) {

        return 0.0;

    }

    // Variable to store total time
    double totalTime = 0.0;

    // Calculate average
    for (double time : jobTimes) {

        totalTime+= time;

    }

    return totalTime / jobTimes.size();

}

// Print result to terminal
void Benchmark::printResult(const std::string& workloadName, int completedJobs) const {

    std::cout << "\n========== BENCHMARK ==========\n";
    std::cout << "Workload: " << workloadName << "\n";
    std::cout << "Completion time: "
              << std::fixed
              << std::setprecision(2)
              << getElapsedMilliseconds()
              << " ms\n";
    std::cout << "Average job time: "
              << std::fixed
              << std::setprecision(2)
              << getAverageJobTime()
              << " ms\n";
    std::cout << "Throughput: "
              << getThroughput(completedJobs)
              << " jobs/second\n";
    std::cout << "================================\n";

}

// Getter for expected workers
int Benchmark::getExpectedWorkers() const {

    return expectedWorkers;

}