#include <iostream>
#include <winsock2.h>
#include "Worker.h"
#include "Client.h"
#include <thread>
#include <functional>

// Method to run worker on thread
void workerThread(Worker& worker) {

    // Connect to port and verify
    if (!worker.connectToScheduler("127.0.0.1", 5000)) {

        std::cerr << "Failed to connect\n";
        return;

    }

    // Register worker with scheduler
    worker.registerWorker();

    // Begin the heartbeat thread
    worker.startHB();

    // Worker stays alive after registering
    while (worker.isRunning()) {

        std::string message = worker.receiveMessages();

        // If empty message then worker connection was closed
        if (message.empty()) {

            break;

        }

        std::cout << "Received message: "
          << message
          << "\n";

        // Process message
        if (message.rfind("JOB|", 0) == 0) {

            // Erase the "JOB|" prefix
            message.erase(0, 4);

            // Retrieve job characteristics and assign to new Job object
            Job job = Job::deserialize(message);

            // Execute and complete job
            worker.startJob(std::move(job));

        }

    }

    std::cout << "Worker thread shutting down!\n";

}

int main() {

    // Ready windows networking library
    WSADATA wsaData;

    // In case windows networking library setup fails
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {

        std::cerr << "WSAStartup failed.\n";
        return 1;

    }

    // Initialise worker objects
    // Note 16384 is 16GB
    Worker worker1("worker1", 8, 10, 16384);
    Worker worker2("worker2", 8, 10, 16384);
    Worker worker3("worker3", 8, 10, 16384);
    Worker worker4("worker4", 8, 10, 16384);

    // Workers may be marked offline for testing purposes

    // Start four threads
    std::thread t1(workerThread, std::ref(worker1));
    std::thread t2(workerThread, std::ref(worker2));
    std::thread t3(workerThread, std::ref(worker3));
    std::thread t4(workerThread, std::ref(worker4));

    std::cout << "Workers started. Press ENTER to shut them down.\n";

    std::cin.get();

    // Shut down each worker following terminal input (Close TCP connection)
    worker1.stop();
    worker2.stop();
    worker3.stop();
    worker4.stop();

    // Stop each of the worker threads
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    // Shut down winsock
    WSACleanup();
    return 0;
    
}