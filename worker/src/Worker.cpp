#include "Worker.h"
#include <algorithm>

// Constructor to initialise Worker object
Worker::Worker(const std::string& workerID, int totalCPUs, int totalGPUs, 
               size_t totalMem)

               :

               // Initialise instance specific variables using constructor:

               // Identification
               workerID(workerID),
               hostName(""),
               IPAddress(""),
               portNumber(""),

               // Resources
               totalCPUs(totalCPUs),
               availableCPUs(totalCPUs),
               totalGPUs(totalGPUs),
               availableGPUs(totalGPUs),
               totalMem(totalMem),
               availableMem(totalMem),

               // State
               state(WorkerState::Idle),

               // Executing job
               currentJobID(""),

               // Worker heartbeat monitoring
               lastHeartbeat(std::chrono::steady_clock::now()),

               // Presets
               jobsCompleted(0),
               jobsFailed(0)

               {
                
               }

// Register worker
void Worker::registerWorker() {

    // Implement in phase 2 when networking is added

}

// Execute job - for phase 1 could end up in partially updated state if exception thrown
bool Worker::executeJob(Job& job) {

    // Return false if insufficient resources
    if (!checkResources(job.getRequiredCPUs(), job.getRequiredGPUs(), job.getRequiredMem())) {

        // Job allocation unsuccessful due to insufficient resources
        return false;

        // Resource allocation failures do not count as execution failures.

    }

    // Assign resources
    assignResources(job.getRequiredCPUs(), job.getRequiredGPUs(), job.getRequiredMem());

    // Update currentJobID
    currentJobID = job.getJobID();

    // Worker becomes busy
    updateState(WorkerState::Busy);

    // Job allocation successful
    return true;

}

// Complete job - for phase 1 could end up in partially updated state if exception thrown
// May not need entire job object for paramter in future
bool Worker::completeJob(Job& job) {

    if (currentJobID != job.getJobID()) {

        return false;

    }

    // Free resources
    freeResources(job.getRequiredCPUs(), job.getRequiredGPUs(), job.getRequiredMem());

    // Reset jobID
    currentJobID = "";

    // Worker becomes idle
    updateState(WorkerState::Idle);

    jobsCompleted++;

    return true;
    
}

// Check resources
bool Worker::checkResources(int requiredCPUs, int requiredGPUs, size_t requiredMem) const {

    if (availableCPUs < requiredCPUs || availableGPUs < requiredGPUs ||
        availableMem < requiredMem) {

            return false;

    }

    return true;

}

// Mark change to available resources
void Worker::assignResources(int requiredCPUs, int requiredGPUs, size_t requiredMem) {

    availableCPUs -= requiredCPUs;
    availableGPUs -= requiredGPUs;
    availableMem -= requiredMem;

}

void Worker::freeResources(int requiredCPUs, int requiredGPUs, size_t requiredMem) {

    availableCPUs = std::min(totalCPUs, availableCPUs + requiredCPUs);
    availableGPUs = std::min(totalGPUs, availableGPUs + requiredGPUs);;
    availableMem = std::min(totalMem, availableMem + requiredMem);;

}

// Update Worker state
void Worker::updateState(WorkerState newStateW) {

    state = newStateW;

}

// Getters and setters for Worker

// Worker id
std::string Worker::getWorkerID() const {

    return workerID;

}

// Current jobID
std::string Worker::getCurrentJobID() const {

    return currentJobID;

}

// Available CPU resources
int Worker::getAvailableCPUs() const {

    return availableCPUs;

}

// Available GPU resources
int Worker::getAvailableGPUs() const {

    return availableGPUs;

}

// Available memory
int Worker::getAvailableMem() const {

    return availableMem;

}

// Current state
WorkerState Worker::getState() const {

    return state;

}