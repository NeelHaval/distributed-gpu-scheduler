#include <iostream>
#include "Scheduler.h"
#include "Worker.h"
#include "Job.h"

int main() {

    // Print to signal scheduler starting
    std::cout << "Starting GPU Scheduler prototype...\n\n";

    // Create scheduler object and print
    Scheduler scheduler;
    std::cout << "[Scheduler] Created scheduler\n";

    // Create workers
    Worker workerA("workerA", 8, 2, 16000);

    // Print workerA details
    std::cout << "[worker] Created worker: workerA\n";
    std::cout << "      CPUs: 8\n";
    std::cout << "      GPUs: 2\n";
    std::cout << "      Memory: 16000MB\n\n";

    // workerA registers with scheduler
    scheduler.registerWorker(workerA);
    std::cout << "[Scheduler] Registered workerA";

    // Create job (GPUS, Mem, CPUs)
    Job job("job1", 1, 4000, 2, "train_model.py");

    // Print to show details
    std::cout << "[Client] Submitted job: job1\n";
    std::cout << "      Requires:\n";
    std::cout << "      CPUs: 2\n";
    std::cout << "      GPUs: 1\n";
    std::cout << "      Memory: 4000\n\n";

    // Submit this job
    scheduler.submitJob(job);
    std::cout << "[Scheduler] Job queued\n\n";

    // Scheduling algorithm
    std::cout << "[Scheduler] Attempting scheduling...\n";
    scheduler.schedule();

    // Complete scheduling
    std::cout << "[Scheduler] Scheduling complete\n\n";

    // Complete job
    std::cout << "[Worker] Finished executing job1\n";

    if (scheduler.completeJob("job1")) {

        std::cout << "[Scheduler] Recorded job completion\n";

    } else {

        std::cout << "[Scheduler] Failed to complete job\n\n";

    }

    // Finish step by step example of scheduler
    std::cout << "GPU Scheduler prototype finished.\n";

    return 0;

}