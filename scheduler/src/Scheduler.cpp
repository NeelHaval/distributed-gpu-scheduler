#include "Scheduler.h"
#include <utility>
#include <iostream>

// Constructor to initialise private variables
Scheduler::Scheduler()

                    :

                    jobsSubmitted(0),
                    jobsCompleted(0),
                    workersRegistered(0),
                    jobsFailed(0)

                    {

                    }

// Register worker - for phase one simply record using registeredWorkers
void Scheduler::registerWorker(const WorkerInfo& worker) {

    auto newWorker = registeredWorkers.emplace(worker.workerID, worker);

    // Increment workersRegistered only if new worker was added
    if (newWorker.second == true) {

        workersRegistered++;

    }
    
    // Note that this method is strictly a phase 1 implementation.
    // It does not handle the possibility of duplicate workers which becomes
    // a real concern in later phases. Therefore, find mechanism to deal with
    // duplicate workers.

}

// Register worker socket
void Scheduler::registerWorkerSocket(const std::string& workerID, Socket socket) {

    // TRIAL
    std::cout << "Registering socket FD: "
              << socket.getFD()
              << "\n";
    // TRIAL

    // Socket cannot be copied
    workerSockets.emplace(workerID, std::move(socket));

}

// Check registered workers for incoming messages
void Scheduler::listenToWorkers() {

    // Check every connected worker
    for (auto& [workerID, socket] : workerSockets) {

        // TRIAL
//         std::cout << "Checking worker socket: "
 //             << workerID << "\n";
        // TRIAL

        // If nothing is waiting continue to the next worker
        while (socket.hasBufferedMessage() || socket.hasData()) {

            // TRIAL
            std::cout << "DATA AVAILABLE FROM: "
                << workerID << "\n";
            // TRIAL

            // Receive message
            std::string message = socket.receive();

            // If receive returned nothing or a partial message then come back another day
            if (message.empty()) {

                break;

            }

            // Print status
            std::cout << "Received from " << workerID << ": " << message << "\n";


            // Handle completed jobs
            if (message.rfind("COMPLETE|", 0) == 0) {

                // Remove the prefix
                std::string jobID = message.substr(9);

                // Print status
                std::cout << "Worker " << workerID << " completed job " << jobID
                          << "\n";

                // Complete the job on the schedulers side
                completeJob(jobID);

            }

        }

    }

}

// Schedulers method to modify the job to submitted
void Scheduler::submitJob(const Job& job) {

    // Trial
    std::cout << "Scheduler::submitJob() called\n";

    // Make a copy of the job
    Job queuedJob = job;

    // Update job state to Queued
    queuedJob.updateState(JobState::Queued);

    // Queue this job
    jobQueue.push(queuedJob);

    // Increment submitted jobs
    jobsSubmitted++;

}

// Send job to worker
bool Scheduler::sendJobWorker(const Job& job) {

    // Find socket associated with worker
    auto socketIt = workerSockets.find(job.getWorkerID());
    
    // If not found signal failure
    if (socketIt == workerSockets.end()) {

        return false;

    }

    // Send job to worker through socket
    std::string message = std::string(Messages::JOB) + "|" + job.serialize();

    ///// TRIAL
    std::cout << "Sending job to worker: " << job.getWorkerID() << "\n";

    std::cout << "Message: " << message << "\n";
    /////

    // Send message to associated worker
    return socketIt->second.send(message);

}

// Complete job
bool Scheduler::completeJob(const std::string& jobID) {

    // Find appropriate job in running jobs
    auto jobIt = runningJobs.find(jobID);

    // If job does not exist return
    if (jobIt == runningJobs.end()) {

        return false;

    }

    // Mark this job completed
    jobIt->second.markCompleted();

    // Move this job to completed jobs
    auto result = completedJobs.emplace(jobID, jobIt->second);

    if (!result.second) {

        return false;

    }

    // Retrieve the worker object which is running this job
    auto workerIt = registeredWorkers.find(jobIt->second.getWorkerID());

    // Error in workerID book keeping
    if (workerIt == registeredWorkers.end()) {

        return false;

    }

    // Update schedulers own record of worker resources
    workerIt->second.availableCPUs += jobIt->second.getRequiredCPUs();
    workerIt->second.availableGPUs += jobIt->second.getRequiredGPUs();
    workerIt->second.availableMem += jobIt->second.getRequiredMem();

    // Worker is idle
    workerIt->second.state = WorkerState::Idle;

    // Remove this job from running jobs
    runningJobs.erase(jobIt);

    // Update jobs completed
    jobsCompleted++;

    // Print queue size before job completed
    std::cout << "Queue size: " << jobQueue.size() << "\n";

    // Print jobs completed
    std::cout << "Jobs completed: " << jobsCompleted << "\n";

    return true;

}

// Phase 1:
// Uses First-Fit scheduling.
//
// Future work:
// Replace with Best-Fit resource-aware scheduling
// to minimise fragmentation and improve utilisation.
// Scheduling algorithm
void Scheduler::schedule() {

    // Trial
// XXXX    std::cout << "Scheduler::schedule() called\n";
// XXXX    std::cout << "Jobs in queue: " << jobQueue.size() << "\n";

    // Counter to record number of jobs processed
    size_t jobsAttempted = jobQueue.size();

    // While there are jobs waiting
    while (jobsAttempted--) {

        // Obtain job from job queue
        Job job = jobQueue.front();
        jobQueue.pop();

        // Find a suitable worker
        WorkerInfo* worker = findAvailableWorker(job);

        //// TRIAL
        if(worker == nullptr) {

            //std::cout << "No available worker found\n";

        }
        else {

            std::cout << "Found worker: "
                    << worker->workerID
                    << "\n";

        }
        //// TRIAL

        // If no suitable worker found
        if ( worker == nullptr) {

            // Re-insert job into queue to prevent loss
            jobQueue.push(job);

            // Try other jobs in queue
            continue;

        // If worker was found
        }

        // Assign specific worker to current job
        job.assignWorker(worker->workerID);

        // As of phase 2 scheduler does not execute jobs
        // Reserve resources in scheduler's worker record
        worker->availableCPUs -= job.getRequiredCPUs();
        worker->availableGPUs -= job.getRequiredGPUs();
        worker->availableMem -= job.getRequiredMem();

        // Mark worker busy
        worker->state = WorkerState::Busy;

        // Mark job running
        job.updateState(JobState::Running);

        // Store running job
        runningJobs.emplace(job.getJobID(), job);

        // Send message
        sendJobWorker(job);

    }

}

// Algorithm for schedule which returns most suitable worker
// Phase 1:
// Return first fit - poor utilization
// Later phases - Improve worker selection algorithm
// Return chosen worker
WorkerInfo* Scheduler::findAvailableWorker(const Job& job) {

    // Iterate through registeredWorkers to find suitable worker
    for (auto& [workerID, worker] : registeredWorkers) {
/*
        // TRIAL
        std::cout << "Worker "
          << workerID
          << " state = "
          << static_cast<int>(worker.state)
          << "\n";
          // TRIAL
*/
        // Find a worker which is currently free
        if (worker.state != WorkerState::Idle) {

            continue;

        }

        // TRIAL
        std::cout << "Worker resources: "
          << "CPU=" << worker.availableCPUs
          << " GPU=" << worker.availableGPUs
          << " MEM=" << worker.availableMem
          << "\n";

        std::cout << "Job requires: "
          << "CPU=" << job.getRequiredCPUs()
          << " GPU=" << job.getRequiredGPUs()
          << " MEM=" << job.getRequiredMem()
          << "\n";
        // TRIAL

        // If worker is free
        if (worker.availableCPUs >= job.getRequiredCPUs() &&
            worker.availableGPUs >= job.getRequiredGPUs() &&
            worker.availableMem >= job.getRequiredMem()) {

            // Must return a pointer
            return& worker;

        }

    }

    // No suitable worker exists
    return nullptr;

}

// Left as a stuf for phase 1 as no way to fail a job
 void Scheduler::failJob(const std::string& jobID) {



}

// Getter for schedulers updated live copy of a job object in runningJobs
const Job* Scheduler::getRunningJob(const std::string& id) const {

    auto it = runningJobs.find(id);

    if (it == runningJobs.end()) {

        return nullptr;

    }

    return &it->second;

}

// Methods for testing
size_t Scheduler::queueSize() const {

    return jobQueue.size();

}

int Scheduler::workerCount() const {

    return registeredWorkers.size();

}

// Getter for submitted and completed jobs
int Scheduler::getJobsSubmitted() const {

    return jobsSubmitted;

}
int Scheduler::getJobsCompleted() const {

    return jobsCompleted;

}