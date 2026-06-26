#include <string>
#include <chrono>

// States which a job may be in
enum class JobState {

    Submitted,
    Queued,
    Scheduling,
    Assigned,
    Dispatching,
    Running,
    Retrying,
    Failed,
    Completed

};

// Priority hierarchy
enum class JobPriority {

    Low,
    Normal,
    High,
    Critical

};

// Class dictating behaviour of jobs
class Job {

    // Job methods
    public:
    
    // Constructor
    Job(std::string jobID, int requiredGPUs, size_t requiredMem, 
    std::string payload, JobPriority priority = JobPriority::Normal);

    // Serialize/Deserialize needed for RPC, networking and logging
    std::string serialize() const;
    static Job deserialize(const std::string data);

    // Job status update
    void updateStatus(JobState newState);

    // Mark failed in case of worker crash or timeout
    void markFailed();

    // Mark complete in case of clean termination path
    void markCompleted();

    // Accessible through class only variables for Jobs
    private:
    std::string jobID;
    std::chrono::time_point<std::chrono::system_clock> submitTime;
    int requiredGPUs;
    int requiredCPUs;
    size_t requiredMem;
    std::string workerID;
    std::string payload;
    int expectedDuration;
    JobPriority priority;
    JobState status;
    int retryCount;

};