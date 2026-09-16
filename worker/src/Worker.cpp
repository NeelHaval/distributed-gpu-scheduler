#include "Worker.h"
#include <algorithm>
#include <iostream>
#include <thread>
#include <chrono>

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

               // Worker heartbeat monitoring
               lastHeartbeat(std::chrono::steady_clock::now()),

               // Presets
               jobsCompleted(0),
               jobsFailed(0)

               {
                
               }

// Destructor
Worker::~Worker() {

    stop();

}

// Method allowing worker to connect to scheduler
bool Worker::connectToScheduler(const std::string& ip, int port) {

    return client.connect(ip, port);

}

// Receive scheduler messages
std::string Worker::receiveMessages() {

    return client.receive();

}

// Register worker
void Worker::registerWorker() {

    // Implement in phase 2 when networking is added
    std::string message = ("REGISTER|" + workerID + "|" + std::to_string(totalCPUs) + "|"
                + std::to_string(totalGPUs) + "|" + std::to_string(totalMem));

    std::cout << "Sending: [" << message << "]\n";

    bool success = client.send(message);

    std::cout << "client.send() returned "
              << (success ? "true" : "false") << "\n";

}

// Execute job - for phase 1 could end up in partially updated state if exception thrown
bool Worker::executeJob(Job& job) {

    // Set up local scope to allow lock_guard object to protect resources
    {

        // Lock_guard object
        std::lock_guard<std::mutex> lock(resourceMutex);

        // Return false if insufficient resources
        if (!checkResources(job.getRequiredCPUs(), job.getRequiredGPUs(), job.getRequiredMem())) {

            // Job allocation unsuccessful due to insufficient resources
            return false;

            // Resource allocation failures do not count as execution failures.

        }

        // Assign resources
        assignResources(job.getRequiredCPUs(), job.getRequiredGPUs(), job.getRequiredMem());

        // Update currentJobID
        activeJobIDs.insert(job.getJobID());

        // Worker becomes busy
        updateState(WorkerState::Busy);

    }

    // Signal to scheduler that job has started
    std::string message = "STARTED|" + job.getJobID();

    sendMessage(message);

    // Simulate delay execution time
    std::this_thread::sleep_for(std::chrono::milliseconds(job.getExecutionTimeMs()));

    // Job allocation successful
    return true;

}

// Complete job - for phase 1 could end up in partially updated state if exception thrown
// May not need entire job object for paramter in future
bool Worker::completeJob(Job& job) {

    // Set up local scope to allow lock_guard object to protect resources
    {

        // Lock_guard object
        std::lock_guard<std::mutex> lock(resourceMutex);

        // If the worker was executing no such job
        if (activeJobIDs.find(job.getJobID()) == activeJobIDs.end()) {

            return false;

        }

        // Erase this job
        activeJobIDs.erase(job.getJobID());

        // Free resources
        freeResources(job.getRequiredCPUs(), job.getRequiredGPUs(), job.getRequiredMem());

        // Worker becomes idle if no jobs being executed
        if (getActiveJobCount() == 0) {

            updateState(WorkerState::Idle);

        }

    }

    jobsCompleted++;

    // Signal to scheduler that job complete
    std::string message = "COMPLETE|" + job.getJobID();
    sendMessage(message);

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
size_t Worker::getActiveJobCount() const {

    return activeJobIDs.size();

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

// Send message with mutex
void Worker::sendMessage(const std::string& message) {

    std::lock_guard<std::mutex> lock(sendMutex);
    client.send(message);

}

// Execute and complete job on its own thread
void Worker::startJob(Job job) {

    jobThreads.emplace_back([this, job]() mutable {

        if (executeJob(job)) {

            completeJob(job);

        }

    });

}

// Stop worker and join all job threads
void Worker::stop() {

    // Stop worker recieve loop
    running = false;

    // Officially disconnect from the scheduler
    client.disconnect();

    // Close heartbeat thread
    if (HBThread.joinable()) {

        HBThread.join();

    }

    // Finish all job threads
    for (std::thread& thread : jobThreads) {

        if (thread.joinable()) {

            thread.join();

        }

    }

    // Remove completed thread objects
    jobThreads.clear();

}

// Check if worker running
bool Worker::isRunning() const {

    return running;

}

// Start the heartbeat thread
void Worker::startHB() {

    HBThread = std::thread(&Worker::HBloop, this);

}

// Send heartbeats
void Worker::HBloop() {

    while (running) {

        // Only if the heartbeat flag for this specific worker is enabled
        // send the message
        if (heartbeat) {

            // Signal heartbeat by sending message
            sendMessage("HEARTBEAT|" + workerID);

        }

        // Send hearbeat at 1 second intervals
        std::this_thread::sleep_for(std::chrono::seconds(1));

    }

    std::cout << "Heartbeat thread stopping for "
              << workerID << "\n";

}

// Set heartbeats
void Worker::setHB(bool mark) {

    heartbeat = mark;

}