#include <iostream>
#include <winsock2.h>
#include <sstream>
#include "Scheduler.h"
#include "Server.h"
#include "WorkerInfo.h"
#include "Job.h"

// Method to parse registration
WorkerInfo parseRegistration(std::string message) {

    // Print the raw message
    std::cout << "Raw message: [" << message << "]\n";

    // Remove register prefix
    message.erase(0, 9);

    // Good for debugging
    std::cout << "After erase: [" << message << "]\n";

    // Read message
    std::stringstream ss(message);

    // Variables to store worker metrics
    std::string workerID;
    std::string cpu;
    std::string gpu;
    std::string mem;

    // Assign
    getline(ss, workerID, '|');
    getline(ss, cpu, '|');
    getline(ss, gpu, '|');
    getline(ss, mem, '|');

    // Print each parsed field
    std::cout << "workerID = [" << workerID << "]\n";
    std::cout << "cpu      = [" << cpu << "]\n";
    std::cout << "gpu      = [" << gpu << "]\n";
    std::cout << "mem      = [" << mem << "]\n";

    // Create worker info struct for this worker which the scheduler records
    WorkerInfo info;

    // Assign appropriate variables in info struct
    info.workerID = workerID;
    info.availableCPUs = std::stoi(cpu);
    info.availableGPUs = std::stoi(gpu);
    info.availableMem = std::stoi(mem);
    info.state = WorkerState::Idle;

    // Return struct
    return info;


}

int main() {

    // Ready windows networking library
    WSADATA wsaData;

    // In case windows networking library setup fails
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {

        std::cerr << "WSAStartup failed.\n";
        return 1;

    }

    // Initialise the scheduler object
    Scheduler scheduler;

    // Create a TCP server which listens exclusively to port 5000
    Server server(5000);

    // Begin listening
    if (!server.start()) {

        std::cerr << "Failed to start server.\n";

        // Stop process and cleanup
        WSACleanup();
        return 1;

    }

    // Print listening port successful status
    std::cout << "Listening to port 5000\n";

    // Jobs to test worker acceptance and rejection of jobs
    Job job1(
        "job1",
        1,          // required GPUs
        1024,       // required memory
        2,          // required CPUs
        "payload1",
        JobPriority::Normal
    );

    Job job2(
        "job2",
        1,
        1024,
        2,
        "payload2",
        JobPriority::Normal
    );

    Job job3(
        "job3",
        1,
        1024,
        2,
        "payload3",
        JobPriority::Normal
    );

    Job job4(
        "job4",
        1,
        1024,
        2,
        "payload4",
        JobPriority::Normal
    );

    scheduler.submitJob(job1);
    scheduler.submitJob(job2);
    scheduler.submitJob(job3);
    scheduler.submitJob(job4);

    // Keep listening for new workers
    while (true) {

// XXXX        std::cout << "Checking for incoming worker...\n";

        // Check for new worker
        if (server.hasIncomingClient()) {

            std::cout << "Incoming worker detected.\n";

            // If present accept
            Socket workerSocket = server.acceptClient();

            // Print status message
            std::cout << "Worker connected.\n";

            // Receive registration
            std::string message = workerSocket.receive();

            // Print status
            std::cout << "Received" << message.size() << " bytes.\n";

            // Perse the registration message
            WorkerInfo worker = parseRegistration(message);

            // Fully register
            scheduler.registerWorker(worker);

            // TRIAL
            std::cout << "REGISTERED WORKER: " << worker.workerID << "\n";
            // TRIAL

            // TRIAL
            std::cout << "Accepted worker socket FD: "
          << workerSocket.getFD()
          << "\n";
          // TRIAL

            // Store the socket associated with workerID
            scheduler.registerWorkerSocket(worker.workerID, std::move(workerSocket));

            // TRIAL
            std::cout << "REGISTERED SOCKET FOR: " << worker.workerID << "\n";
            // TRIAL

            // Print status
            std::cout << "Registered worker: " << worker.workerID << "\n";

            // Trial: Assign different jobs to workers
            std::string jobID = "job_" + worker.workerID;

            // Test job TRIAL ONLY TRIAL ONLY TRIAL ONLY
            /*
            Job testJob(
                jobID,
                1,          // required GPUs
                1024,       // required memory MB
                2,          // required CPUs
                "test_payload",
                JobPriority::Normal
            );
            scheduler.submitJob(testJob);
            */

        }

        // Continue checking for existing worker messages
        scheduler.listenToWorkers();

        // Schedule waiting jobs
        scheduler.schedule();

    }

    // Shut down winsock
    WSACleanup();

    // Exit program
    return 0;

}