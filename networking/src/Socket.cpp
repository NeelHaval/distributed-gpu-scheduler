#include "Socket.h"
#include <unistd.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

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

    std::cout << "Socket::send() called\n";
    std::cout << "Sending data: [" << data << "]\n";

    // Mark the end of the protocol message
    std::string frameData = data + "\n";

    const char* c = frameData.c_str();
    int nBytes = static_cast<int>(frameData.size());

    int sent = 0;

    while (sent < nBytes) {

        int result = ::send(socketFD,
                            c + sent,
                            nBytes - sent,
                            0);

        std::cout << "::send returned " << result << "\n";

        if (result == SOCKET_ERROR) {

            std::cout << "WSA Error = "
                      << WSAGetLastError()
                      << "\n";

            return false;
        }

        sent += result;
    }

    return true;

}

// Method to receive protocol messages
std::string Socket::receive() {

    // Served buffered messages before conducting a syscall
    size_t delimiter = receiveBuffer.find('\n');

    // If no newline then full message not received
    if (delimiter == std::string::npos) {

        // Receive raw bytes
        char buffer[4096];
        int bytesReceived = ::recv(socketFD, buffer, sizeof(buffer), 0);

        // If no bytes received return empty string
        if (bytesReceived <= 0) {

            return "";

        }

        // Append new byte stream received to receiveBuffer
        receiveBuffer.append(buffer, bytesReceived);

        // Find delimiter
        delimiter = receiveBuffer.find('\n');

        // If full message still not received come back another working day
        if (delimiter == std::string::npos) {

            return "";

        }

    }

    // Extract single message
    std::string message = receiveBuffer.substr(0, delimiter);
    receiveBuffer.erase(0, delimiter + 1);
    return message;

}

// Check whether there is data waiting to be read
bool Socket::hasData() const {

    // Make a set containing this socket
    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(socketFD, &readSet);

    // Do not wait for check
    timeval timeout{};
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    // Use winsock to check whether socket is readable
    int result = ::select(0, &readSet, nullptr,
                          nullptr, &timeout);

    // TRIAL
 //   std::cout << "hasData(): socketFD = "
 //             << socketFD
  //            << ", select result = "
  //            << result
 //             << "\n";
    // TRIAL

    // Report error
    if (result == SOCKET_ERROR) {

        // TRIAL
        std::cout << "select error = "
                  << WSAGetLastError()
                  << "\n";
        // TRIAL

        return false;

    }

    // If the socket was successfully added to the read set then 
    // data is available
    return FD_ISSET(socketFD, &readSet);

}

// Check whether buffered data is waiting
bool Socket::hasBufferedMessage() const {

    return receiveBuffer.find('\n') != std::string::npos;

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

// Debugging getters

// Socket.cpp
int Socket::getFD() const {
    return socketFD;
}

/*

Should go inside Socket::send:

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

*/

/* Old receive method

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

*/

/*

Old receive method

// Temporary buffer to capture bytes that were received
    char buffer[4096];

    // Receive the above bytes
    int bytesReceived = ::recv(socketFD, buffer, sizeof(buffer), 0);

    // If error occurs
    if (bytesReceived <= 0)  {

        // Return empty string to signal error
        return "";

    }

    // Add above bytes to the final buffer
    receiveBuffer.append(buffer, bytesReceived);

    // Check for new line (delimiter)
    size_t delimiter = receiveBuffer.find('\n');

    // If no delimiter found them message has not been fully transmitted
    if (delimiter == std::string::npos) {

        return "";

    }

    // Otherwise extract the entire message (everything before the delimiter)
    std::string message = receiveBuffer.substr(0, delimiter);

    // Remove the entire message and its delimiter from buffer
    receiveBuffer.erase(0, delimiter + 1);

    // Return the full single protocol message
    return message;

*/