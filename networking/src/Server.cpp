#include "Server.h"

// Constructor to initialise server object
Server::Server(int port)

            :

            port(port)

            {

            }

// Begin listening
bool Server::start() {

    // If server listening socket creation was unsuccessful
    if (!listenSocket.create()) {

        return false;

    }

    // If port was not assigned to listening socket
    if (!listenSocket.bind(port)) {

        return false;

    }

    // Check if OS is waiting for incomming TCP connections
    if (!listenSocket.listen()) {

        return false;

    }

    // If listenSocket is correctly initialised signal success
    return true;

}

// Connect to client
Socket Server::acceptClient() {

    return listenSocket.accept();

}

// Check whether a new client (worker) is waiting to connect
bool Server::hasIncomingClient() const {

    return listenSocket.hasData();

}

// Stop server operation and close listenSocket
void Server::stop() {

    listenSocket.close();

}