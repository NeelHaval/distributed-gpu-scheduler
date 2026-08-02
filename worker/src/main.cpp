#include <iostream>
#include <winsock.h>
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

    // Initialise client object
    Client client;

    // Connect to port and verify
    if (!client.connect("127.0.0.1", 5000)) {

        std::cerr << "Failed to connect\n";
        WSACleanup();
        return 1;

    }

    // Signal that connected to scheduler
    std::cout << "Connected to scheduler.\n";

    // Shut down winsock
    WSACleanup();
    return 0;
    
}