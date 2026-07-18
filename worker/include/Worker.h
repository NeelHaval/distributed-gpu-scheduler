#include <string>
#include <chrono>
#include "Job.h"

// Worker state
enum class WorkerState {

    Idle,
    Busy,
    Offline,
    Registering

};

// Class interface
class Worker {

    public:

    // Constructor
    Worker(const std::string& workerID, int totalCPUs, int totalGPUs, size_t totalMem);

    // Required Methods:

    // Worker registers with scheduler
    void registerWorker();

    // Worker executes job
    bool executeJob(Job& job);

    // Worker completes job
    bool completeJob(Job& job);

    // Update worker state
    void updateState(WorkerState newStateW);

    // Verify existence of resources
    bool checkResources(int requiredCPUs, int requiredGPUs, size_t requiredMem);

    // Reserve resources
    void assignResources(int requiredCPUs, int requiredGPUs, size_t requiredMem);

    // Free resources
    void freeResources(int requiredCPUs, int requiredGPUs, size_t requiredMem);

    // Worker id
    std::string getWorkerID() const;

    // Available CPU resources
    int getAvailableCPUs() const;

    // Available GPU resources
    int getAvailableGPUs() const;

    // Available memory
    int getAvailableMem() const;

    // Current state
    WorkerState getState() const;

    /* 
    Later add these methods:
    sendHeartbeat()
    markOffline()
    connectToScheduler()
    */

    private:

    // Identity parameters
    std::string workerID;
    std::string hostName;
    std::string IPAddress;
    std::string portNumber;

    // Current job
    std::string currentJobID;

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