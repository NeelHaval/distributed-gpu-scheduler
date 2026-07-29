#include "Client.h"

// Constructor to initialise client object
Client::Client()

            {

            }

// Connect to a port
bool Client::connect(const std::string& ip, int port) {

    return socket.connect(ip, port);

}

// Send protocol message over TCP connection
bool Client::send(const std::string& data) {

    return socket.send(data);

}

// Read message
std::string Client::receive() {

    return socket.receive();

}

// Close TCP connection
void Client::disconnect() {

    socket.close();

}