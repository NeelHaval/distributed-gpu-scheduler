#pragma once
#include <string>
#include <unordered_map>
#include <queue>
#include "Job.h"
#include "WorkerInfo.h"
#include "Socket.h"
#include "Messages.h"
#include <chrono>

// Benchmark class decleration
class Benchmark;

// Scheduler class interface
class Scheduler {

    public:

    // Connect benchmark to scheduler
    void setBenchmark(Benchmark* benchmark);

    // Constructor
    Scheduler();

    // Register workers
    void registerWorker(const WorkerInfo& worker);

    // Register worker socket
    void registerWorkerSocket(const std::string& workerID, Socket socket);

    // Check registered workers for incoming messages
    void listenToWorkers();

    // Push jobs into queue
    void submitJob(const Job& job);

    // Send job to worker
    bool sendJobWorker(const Job& job);

    // Scheduling algorithm
    void schedule();

    // Mark a job complete
    bool completeJob(const std::string& jobID);

    // Recover failed jobs
    void failJob(const std::string& jobID);

    // Return chosen worker
    WorkerInfo* findAvailableWorker(const Job& job);

    // Required for testing
    size_t queueSize() const;
    int workerCount() const;

    // Getter for schedulers updated live copy of a job object in runningJobs
    const Job* getRunningJob(const std::string& id) const;

    // Get jobs submitted and completed
    int getJobsSubmitted() const;
    int getJobsCompleted() const;

    private:

    // Benchmark used to hold job execution times
    Benchmark* benchmark;

    // Begin recording for each job
    std::unordered_map<std::string, std::chrono::steady_clock::time_point> jobStartTimes;

    // Vector containing current registered workers
    std::unordered_map<std::string, WorkerInfo> registeredWorkers;

    // Save sockets associated with workers
    std::unordered_map<std::string, Socket> workerSockets;

    // Phase 1 stores Jobs by value.
    // Later phases should replace this with shared_ptr<Job>
    // to avoid multiple copies across scheduler state.
    // Jobs currently in queue
    std::queue<Job> jobQueue;

    // Current running jobs
    std::unordered_map<std::string, Job> runningJobs;

    // Completed jobs
    std::unordered_map<std::string, Job> completedJobs;

    // Statistics
    size_t jobsSubmitted;
    size_t jobsCompleted;
    size_t workersRegistered;
    size_t jobsFailed;

};