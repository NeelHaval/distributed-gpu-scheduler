#include <string>
#include <unordered_map>
#include <queue>
#include "Job.h"
#include "Worker.h"

// Class interface
class Scheduler {

    public:

    // Constructor
    Scheduler();

    // Register workers
    void registerWorker(const Worker& worker);

    // Push jobs into queue
    void submitJob(const Job& job);

    // Scheduling algorithm
    void schedule();

    // Mark a job complete
    void completeJob(const std::string& jobID);

    // Recover failed jobs
    void failJob(const std::string& jobID);

    // Return chosen worker
    Worker* findAvailableWorker(const Job& job);

    // Required for testing
    int queueSize() const;
    int workerCount() const;

    private:

    // Vector containing current registered workers
    std::unordered_map<std::string, Worker> registeredWorkers;

    // Jobs currently in queue
    std::queue<Job> jobQueue;

    // Current running jobs
    std::unordered_map<std::string, Job> runningJobs;

    // Completed jobs
    std::unordered_map<std::string, Job> completedJobs;

    // Statistics
    int jobsSubmitted;
    int jobsCompleted;
    int workersRegistered;
    int jobsFailed;

};