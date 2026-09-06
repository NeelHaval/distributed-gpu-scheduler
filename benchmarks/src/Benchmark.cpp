#include "Benchmark.h"
#include <iostream>
#include <iomanip>

// Constructor for benchmark
Benchmark::Benchmark(int expectedWorkers, int totalCPUsPerWorker, int totalGPUsPerWorker,
              size_t totalMemPerWorker)

    : 
    
    expectedWorkers(expectedWorkers),
    totalCPUsPerWorker(totalCPUsPerWorker),
    totalGPUsPerWorker(totalGPUsPerWorker),
    totalMemPerWorker(totalMemPerWorker),
    cpuUsage(0.0),
    gpuUsage(0.0),
    memoryUsage(0.0)

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
void Benchmark::recordJobTime(const std::string& jobID, double durationMs, int requiredCPUs,
                       int requiredGPUs, size_t requiredMem) {

    jobTimes.push_back(durationMs);

    cpuUsage += static_cast<double>(requiredCPUs) * durationMs;

    gpuUsage += static_cast<double>(requiredGPUs) * durationMs;

    memoryUsage += static_cast<double>(requiredMem) * durationMs;

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

    std::cout << "CPU utilisation: "
              << getCPUUtil()
              << " %\n";

    std::cout << "GPU utilisation: "
              << getGPUUtil()
              << " %\n";

    std::cout << "Memory utilisation: "
              << getMemoryUtil()
              << " %\n";

    std::cout << "Average queueing time: "
              << getAverageQueueTime()
              << " ms\n";

    std::cout << "================================\n";

}

// Get resource - level worker utilisation
double Benchmark::getCPUUtil() const {

    // Check for errors
    if (expectedWorkers <= 0 || totalCPUsPerWorker <= 0) {

        return 0.0;

    }

    // Total CPU time
    double totalAvailableCPUTime = static_cast<double>(expectedWorkers) * totalCPUsPerWorker
                                    * getElapsedMilliseconds();

    if (totalAvailableCPUTime <= 0.0) {

        return 0.0;

    }

    // Return benchmark metric
    return (cpuUsage / totalAvailableCPUTime) * 100.0;
}

double Benchmark::getGPUUtil() const {

    // Check for errors
    if (expectedWorkers <= 0 || totalGPUsPerWorker <= 0) {

        return 0.0;

    }

    // Total GPU time
    double totalAvailableGPUTime = static_cast<double>(expectedWorkers) * totalGPUsPerWorker
        * getElapsedMilliseconds();

    if (totalAvailableGPUTime <= 0.0) {

        return 0.0;

    }

    // Return benchmark metric
    return (gpuUsage / totalAvailableGPUTime) * 100.0;
}

double Benchmark::getMemoryUtil() const {

    // Check for errors
    if (expectedWorkers <= 0 || totalMemPerWorker == 0) {

        return 0.0;

    }

    // Total Memory time
    double totalAvailableMemoryTime = static_cast<double>(expectedWorkers) * static_cast<double>(totalMemPerWorker)
        * getElapsedMilliseconds();

    if (totalAvailableMemoryTime <= 0.0) {

        return 0.0;

    }

    // Return benchmark metric
    return (memoryUsage / totalAvailableMemoryTime) * 100.0;
}

// Record the queueing time for a job
void Benchmark::recordQueueTime(const std::string& jobID, double durationMs) {

    queueTimes.push_back(durationMs);

}

// Calculate average queueing time
double Benchmark::getAverageQueueTime() const {

    if (queueTimes.empty()) {

        return 0.0;

    }

    // Variable to hold total queueing time
    double total = 0.0;

    // Calculate average and return appropriate value
    for (double time : queueTimes) {

        total += time;

    }

    return total / queueTimes.size();

}

// Getter for expected workers
int Benchmark::getExpectedWorkers() const {

    return expectedWorkers;

}