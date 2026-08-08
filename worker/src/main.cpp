#include <iostream>
#include <winsock2.h>
#include "Worker.h"
#include "Client.h"

int main() {

    // Ready windows networking library
    WSADATA wsaData;

    // In case windows networking library setup fails
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {

        std::cerr << "WSAStartup failed.\n";
        return 1;

    }

    // Initialise worker object
    // Note 16384 is 16GB
    Worker worker("worker1", 8, 1, 16384);

    // Connect to port and verify
    if (!worker.connectToScheduler("127.0.0.1", 5000)) {

        std::cerr << "Failed to connect\n";
        WSACleanup();
        return 1;

    }

    // Register worker with scheduler
    worker.registerWorker();

    // Worker stays alive after registering
    while (true) {

        std::string message = worker.receiveMessages();

        std::cout << "Received message: "
          << message
          << "\n";

        // Process message
        if (message.rfind("JOB|", 0) == 0) {

            // Erase the "JOB|" prefix
            message.erase(0, 4);

            // Retrieve job characteristics and assign to new Job object
            Job job = Job::deserialize(message);

            // Execute job
            worker.executeJob(job);

            // Signal complete
            worker.completeJob(job);

        }

    }

    // Signal that connected to scheduler
    std::cout << "Connected to scheduler.\n";

    // Shut down winsock
    WSACleanup();
    return 0;
    
}