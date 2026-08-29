#pragma once
#include <chrono>
#include <string>
#include <vector>

// Create Benchmark class
class Benchmark {

// Public methods
public:

    // Constructor for benchmark object
    Benchmark(int expectedWorkers);

    // Mark start time
    void start();

    // Mark stop time
    void stop();

    // Calculate elapsed time
    double getElapsedMilliseconds() const;

    // Print result to terminal
    void printResult(const std::string& workloadName) const;

    // Getter for expected workers
    int getExpectedWorkers() const;

    // Calculate the job execution time
    void recordJobTime(const std::string& jobID, double durationMs);

    // Calculate average job execution metric
    double getAverageJobTime() const;

// Private variables
private:

    // Number of expected workers
    int expectedWorkers;

    // To record start and end times
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point endTime;

    // Store the measured execution time for each job in the respective entry
    std::vector<double> jobTimes;

};