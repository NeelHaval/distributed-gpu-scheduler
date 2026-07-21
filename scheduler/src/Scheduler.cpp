#include "Scheduler.h"

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
void Scheduler::registerWorker(const Worker& worker) {

    auto newWorker = registeredWorkers.emplace(worker.getWorkerID(), worker);

    // Increment workersRegistered only if new worker was added
    if (newWorker.second == true) {

        workersRegistered++;

    }
    
    // Note that this method is strictly a phase 1 implementation.
    // It does not handle the possibility of duplicate workers which becomes
    // a real concern in later phases. Therefore, find mechanism to deal with
    // duplicate workers.

}

// Schedulers method to modify the job to submitted
void Scheduler::submitJob(const Job& job) {

    // Make a copy of the job
    Job queuedJob = job;

    // Update job state to Queued
    queuedJob.updateState(JobState::Queued);

    // Queue this job
    jobQueue.push(queuedJob);

    // Increment submitted jobs
    jobsSubmitted++;

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

    // Move this job completed jobs
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

    // Call on worker class completeJob method
    workerIt->second.completeJob(jobIt->second);

    // Remove this job from running jobs
    runningJobs.erase(jobIt);

    // Update jobs completed
    jobsCompleted++;

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

    // Counter to record number of jobs processed
    size_t jobsAttempted = jobQueue.size();

    // While there are jobs waiting
    while (jobsAttempted--) {

        // Obtain job from job queue
        Job job = jobQueue.front();
        jobQueue.pop();

        // Find a suitable worker
        Worker* worker = findAvailableWorker(job);

        // If no suitable worker found
        if ( worker == nullptr) {

            // Re-insert job into queue to prevent loss
            jobQueue.push(job);

            // Try other jobs in queue
            continue;

        // If worker was found
        }

        // Defensive execution check in case worker state changed
        // between scheduling and execution
        if ((*worker).executeJob(job)) {

            // Run job - note that this may have to change later as project networks
            // more i.e. enter Queued and scheduling states
            job.updateState(JobState::Running);

            // Insert into runningJobs
            auto result = runningJobs.emplace(job.getJobID(), job);

            if (!result.second) {

                return;

            }

        } else {

            // Re-insert job into queue to prevent loss
            jobQueue.push(job);

        }

    }

}

// Algorithm for schedule which returns most suitable worker
// Phase 1:
// Return first fit - poor utilization
// Later phases - Improve worker selection algorithm
// Return chosen worker
Worker* Scheduler::findAvailableWorker(const Job& job) {

    // Iterate through registeredWorkers to find suitable worker
    for (auto& [workerID, worker] : registeredWorkers) {

        // Find a worker which is currently free
        if (worker.getState() != WorkerState::Idle) {

            continue;

        }

        // If worker is free
        if (worker.checkResources(job.getRequiredCPUs(), job.getRequiredGPUs(),
            job.getRequiredMem())) {

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

// Methods for testing
size_t Scheduler::queueSize() const {

    return jobQueue.size();

}

int Scheduler::workerCount() const {

    return registeredWorkers.size();

}