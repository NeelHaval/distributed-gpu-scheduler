#pragma once
#include <string>
#include <chrono>
#include "Job.h"
#include "Client.h"
#include "WorkerState.h"
#include <unordered_set>
#include <mutex>
#include <vector>
#include <thread>
#include <atomic>

/*
// Worker state
enum class WorkerState {

    Idle,
    Busy,
    Offline,
    Registering

};
*/
// Class interface
class Worker {

    public:

    // Method allowing worker to connect to scheduler
    bool connectToScheduler(const std::string& ip, int port);

    // Constructor
    Worker(const std::string& workerID, int totalCPUs, int totalGPUs, size_t totalMem);

    // Destructor
    ~Worker();

    // Required Methods:

    // Worker registers with scheduler
    void registerWorker();

    // Receive scheduler messages
    std::string receiveMessages();

    // Worker executes job
    bool executeJob(Job& job);

    // Worker completes job
    bool completeJob(Job& job);

    // Update worker state
    void updateState(WorkerState newStateW);

    // Verify existence of resources
    bool checkResources(int requiredCPUs, int requiredGPUs, size_t requiredMem) const;

    // Reserve resources
    void assignResources(int requiredCPUs, int requiredGPUs, size_t requiredMem);

    // Free resources
    void freeResources(int requiredCPUs, int requiredGPUs, size_t requiredMem);

    // Worker id
    std::string getWorkerID() const;

    // Current jobID
    size_t getActiveJobCount() const;

    // Available CPU resources
    int getAvailableCPUs() const;

    // Available GPU resources
    int getAvailableGPUs() const;

    // Available memory
    int getAvailableMem() const;

    // Current state
    WorkerState getState() const;

    // Send message with mutex
    void sendMessage(const std::string& message);
    
    // Execute and complete job on its own thread
    void startJob(Job job);

    // Stop worker and join all job threads
    void stop();

    // Check if a worker is currently running
    bool isRunning() const;

    /* 
    Later add these methods:
    sendHeartbeat()
    markOffline()
    connectToScheduler()
    */

    private:

    // Worker owns client object
    Client client;

    // Identity parameters
    std::string workerID;
    std::string hostName;
    std::string IPAddress;
    std::string portNumber;

    // Current jobs
    std::unordered_set<std::string> activeJobIDs;

    // Protects resources and active jobs
    std::mutex resourceMutex;

    // Prevents multiple jobs threads from writing to the TCP connection at the
    // same time
    std::mutex sendMutex;

    // Keep execution threads alive instead of detaching them
    std::vector<std::thread> jobThreads;

    // Control worker shut down
    std::atomic<bool> running{true};

    // Resources
    int totalCPUs;
    int availableCPUs;
    int totalGPUs;
    int availableGPUs;
    size_t totalMem;
    size_t availableMem;

    // State
    WorkerState state;

    // Last Heartbeat
    std::chrono::time_point<std::chrono::steady_clock> lastHeartbeat;

    // Statics
    int jobsCompleted;

    // Phase 6:
    // Increment when a job fails during execution.
    int jobsFailed;
    
    // Note that jobs executed is jobs failed + jobs completed

};