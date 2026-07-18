#include "Job.h"
#include <sstream>

// Note that later implementations could add string validation before conversions

// Constructor to initialise Job object
Job::Job(const std::string& jobID, int requiredGPUs, size_t requiredMem, int requiredCPUs,
    const std::string& payload, JobPriority priority)

    :

    // Initalise instance specific variables using constructor
    jobID(jobID),
    submissionTime(std::chrono::steady_clock::now()),
    requiredGPUs(requiredGPUs),
    requiredMem(requiredMem),
    requiredCPUs(requiredCPUs),
    payload(payload),
    priority(priority),
    state(JobState::Submitted),
    retryCount(0),
    expectedDuration(0),
    workerID("")

    {

    }

// State update
void Job::updateState(JobState newStateJ) {

    state = newStateJ;

}

// Mark completed and record completion time
void Job::markCompleted() {

    state = JobState::Completed;
    duration = std::chrono::steady_clock::now();

}

// Mark failed and record time wasted
void Job::markFailed() {

    state = JobState::Failed;
    duration = std::chrono::steady_clock::now();

}

// Serialize
std::string Job::serialize() const {

    // Use Delimiting string to serialize Job string
    return jobID + "|" + std::to_string(requiredGPUs) + "|" + std::to_string(requiredCPUs)
     + "|" + std::to_string(requiredMem) + "|" + payload
     + "|" + std::to_string(static_cast<int>(priority))
     + "|" + std::to_string(static_cast<int>(state))
     + "|" + std::to_string(retryCount) + "|" + std::to_string(expectedDuration)
     + "|" + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(
               submissionTime.time_since_epoch()).count());

}

// Deserialize the pipe separated string
Job Job::deserialize(const std::string& data) {

    // Wrap string into stream object
    std::stringstream ss(data);

    // Create vector to store individual parts of stream
    std::string token;

    // Extract information and store using appropriate conversions:

    // JobID
    std::getline(ss, token, '|');
    std::string JobID = token;

    // requiredGPUs
    std::getline(ss, token, '|');
    int requiredGPUs = std::stoi(token);

    // requiredCPUs
    std::getline(ss, token, '|');
    int requiredCPUs = std::stoi(token);

    // requiredMem
    std::getline(ss, token, '|');
    size_t requiredMem = static_cast<size_t>(std::stoull(token));

    // payload
    std::getline(ss, token, '|');
    std::string payload = token;

    // priority
    std::getline(ss, token, '|');
    JobPriority priority = static_cast<JobPriority>(std::stoi(token));

    // Create instance of Job using constructor to populate above basic variables
    Job job(JobID, requiredGPUs, requiredMem, requiredCPUs, payload, priority);

    // state
    std::getline(ss, token, '|');
    job.state = static_cast<JobState>(std::stoi(token));

    // retryCount
    std::getline(ss, token, '|');
    job.retryCount = std::stoi(token);

    // expectedDuration
    std::getline(ss, token, '|');
    job.expectedDuration = std::stoi(token);

    // submissionTime
    std::getline(ss, token, '|');
    long long millisec = std::stoll(token);
    job.submissionTime = std::chrono::time_point<std::chrono::steady_clock>(
        std::chrono::milliseconds(millisec));

    // Return packaged job object
    return job;

}

// Getters

// JobID getter
std::string Job::getJobID() const {

    return jobID;

}

// Get required CPUs
int Job::getRequiredCPUs() const {

    return requiredCPUs;

}

// Get requiredGPUs
int Job::getRequiredGPUs() const {

    return requiredGPUs;

}

// Get requiredMem
int Job::getRequiredMem() const {

    return requiredMem;

}

// Assign worker ID to job
void Job::assignWorker(const std::string& data) {

    workerID = data;

}