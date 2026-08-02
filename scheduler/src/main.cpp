#include <iostream>
#include <winsock2.h>
#include "Scheduler.h"
#include "Server.h"

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
    std::cout << "Listening to port 5000";

    // Listen for connecting worker
    Socket workerSocket = server.acceptClient();

    // Print worker connected status
    std::cout << "Worker connected.";

    // Shut down winsock
    WSACleanup();

    // Exit program
    return 0;

}