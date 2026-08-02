#include "Socket.h"
#include <unistd.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// Constructor to initialize Socket object
Socket::Socket() : 

            // SocketFD does not represent a valid socket yet
            socketFD(-1)

            {

            }

// Destructor to close socket not currently in use
Socket::~Socket() {

    // Check if socket represents valid TCP connection
    if (socketFD != -1) {

        // If so close
        closesocket(socketFD);

        // Set to -1
        socketFD = -1;

    }

}

// Wrap system level OS handle to socket object
Socket::Socket(int fd)

                :

                socketFD(fd)

                {
                
                }

// Move constructor to transfer socket
// Note that here other is used as a reference to the ravlue socket object to allow
// reliable moving of the object characteristics
Socket::Socket(Socket&& other) :

            socketFD(other.socketFD)

            {

                other.socketFD = -1;

            }

// Move assignment
Socket& Socket::operator=(Socket&& other) {

    // Check if move assignment is occuring to same object
    if (this != &other) {

        // Clost initial socket
        if (socketFD != -1) {

            closesocket(socketFD);
            socketFD = -1;

        }

        socketFD = other.socketFD;
        other.socketFD = -1;

    }

    return *this;

}

// Method to send scheduler protocol messages
bool Socket::send(const std::string& data) {

    // Extract characters
    const char* c = data.c_str();

    // Extract number of bytes required
    int nBytes = data.size();

    // Variable to hold sent result
    int sent = 0;

    // Variable to hold current sent
    int result;

    while (sent < nBytes) {

        // Send this information using the 
        result = ::send(socketFD, c + sent, nBytes - sent, 0);

        // Verify is success
        if (result == SOCKET_ERROR) {

            // Signal success
            return false;

        }

        // Increment total sent counter
        sent += result;

    }

    return true;

}

// Note that this method does not support preserving messages. This must be solved
// in future phases using message framing.
// Method to receive protocol messages
std::string Socket::receive() {

    // Store incomming bytes in buffer
    char buffer[4096];

    // Request data from winsock function
    int bytesReceived = ::recv(socketFD, buffer, sizeof(buffer), 0);

    // Check failure
    if (bytesReceived <= 0) {

        return "";

    }

    // Return full string
    return std::string(buffer, bytesReceived);

}
//////////
// Connect implementation
bool Socket::connect(const std::string& ip, int port) {

    // Create socket
    socketFD = ::socket(AF_INET, SOCK_STREAM, 0);

    // Verify success in socket creation
    if (socketFD == INVALID_SOCKET) {

        return false;

    }

    // Address structure ***
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);

    // Extract binary form ***
    if (inet_pton(AF_INET, ip.c_str(), &serverAddress.sin_addr) <= 0) {

        closesocket(socketFD);
        socketFD = -1;
        return false;

    }

    // Call connect ***
    if (::connect(socketFD, (sockaddr*)&serverAddress, sizeof(serverAddress))
        == SOCKET_ERROR) {

            closesocket(socketFD);
            socketFD = -1;
            return false;

    }

    // Otherwise return
    return true;

}
//////////
// Clost socket
void Socket::close() {

    if (socketFD != -1) {

        closesocket(socketFD);
        socketFD = -1;

    }

}

// Methods allowing a socket to listen:

// Create TCP socket
bool Socket::create() {

    // Create socket
    socketFD = ::socket(AF_INET, SOCK_STREAM, 0);

    // Verify success in socket creation
    if (socketFD == INVALID_SOCKET) {

        return false;

    }

    return true;

}

// Bind socket to a local port
bool Socket::bind(int port) {

    // Address structure ***
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    // Extract binary form ***
    if (::bind(socketFD, reinterpret_cast<sockaddr*>(&address), 
    sizeof(address)) == SOCKET_ERROR) {

        return false;

    }

    // Otherwise return
    return true;

}

// Put socket into listening mode
bool Socket::listen() {

    // Check if listen socket creation succeeds
    if (::listen(socketFD, SOMAXCONN) == SOCKET_ERROR) {

        return false;

    }

    return true;

}

// Accept an incomming connection
Socket Socket::accept() {

    // Acquire raw system level OS handle
    int clientFD = ::accept(socketFD, nullptr, nullptr);

    // Check if valid
    if (clientFD == INVALID_SOCKET) {

        return Socket();

    }

    // If valid wrap socket with OS handle
    return Socket(clientFD);

}