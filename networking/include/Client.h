#pragma once
#include <string>
#include "Socket.h"

class Client {

    public:

    // Constructor
    Client();

    // Connect to a port
    bool connect(const std::string& ip, int port);

    // Send protocol message over TCP connection
    bool send(const std::string& data);

    // Read message
    std::string receive();

    // Close TCP connection
    void disconnect();

    private:

    Socket socket;

};