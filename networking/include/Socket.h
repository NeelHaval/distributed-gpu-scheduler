#pragma once
#include <string>

// Socket class facilitating networking layer
class Socket {

    public:

    // Constructor and destructor
    Socket();
    ~Socket();

    // Wrap system level OS handle to socket object
    // Change socketFD type to SOCKET later
    explicit Socket(int fd);

    // Prevent copying
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    // Allow moving from one socket object to another
    Socket(Socket&& other);

    // Safely override the old socket object when its place is taken by a new one
    Socket& operator=(Socket&& other);

    // Connection management
    bool connect(const std::string& ip, int port);
    void close();

    // Methods to send and receive scheduler protocol messages
    bool send(const std::string& data);
    std::string receive();

    // Methods allowing a socket to listen:

    // Create TCP socket
    bool create();

    // Bind socket to a local port
    bool bind(int port);

    // Put socket into listening mode
    bool listen();

    // Accept an incomming connection
    Socket accept();

    private:

    // Variable to represent TCP connections
    int socketFD; // Set this to -1 in the constructor

    // Note that socketFD should be changed from int to SOCKET 
    // as this is better compatible with winsock.

};

// Remember to call WSAStartup(...)