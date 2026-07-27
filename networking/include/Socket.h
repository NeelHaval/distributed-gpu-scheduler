#pragma once
#include <string>

// Socket class facilitating networking layer
class Socket {

    public:

    // Constructor and destructor
    Socket();
    ~Socket();

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    // Allow moving
    Socket(Socket&& other);
    Socket& operator=(Socket&& other);

    // Method to send scheduler protocol messages
    bool send(const std::string& data);
    std::string receive();

    private:

    // Variable to represent TCP connections
    int socketFD; // Set this to -1 in the constructor

};