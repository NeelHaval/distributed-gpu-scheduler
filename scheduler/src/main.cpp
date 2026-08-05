#include <iostream>
#include <winsock2.h>
#include <sstream>
#include "Scheduler.h"
#include "Server.h"
#include "WorkerInfo.h"

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

    // Keep listening for new workers
    while (true) {

        // Listen for connecting worker
        Socket workerSocket = server.acceptClient();

        // Print worker connected status
        std::cout << "Worker connected.\n";

        // Revieve message by worker and print to terminal
        std::string message = workerSocket.receive();
        std::cout << "Received " << message.size() << " bytes.\n";

        // Parse registration
        WorkerInfo worker = parseRegistration(message);

        // Register worker with scheduler
        scheduler.registerWorker(worker);

        // Print status
        std::cout << "Registered worker: " << worker.workerID << "\n";

    }

    // Shut down winsock
    WSACleanup();

    // Exit program
    return 0;

}