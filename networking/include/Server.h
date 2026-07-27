#pragma once
#include "Socket.h"

class Server {

    public:

    // Constructor initialising address
    Server(int port);

    // Begin server process
    bool start();

    // Accept client
    Socket acceptClient();

    // Stop process
    void stop();

    private:

    // Variables for address on machine
    int port;
    Socket listenSocket;

};