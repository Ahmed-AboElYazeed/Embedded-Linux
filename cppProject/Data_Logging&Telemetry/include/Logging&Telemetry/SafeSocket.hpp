#pragma once

#include <iostream>
#include <string>
#include <memory>

#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/un.h>

// typedef enum{
//     SERVER,
//     CLIENT
// }SafeSocket_type;

class SafeSocket
{
private:  
    // SafeSocket_type socket_type;
    const char * socket_path;
    std::unique_ptr<int> server_fd_uptr;
    std::unique_ptr<int> client_fd_uptr;
    std::unique_ptr <struct sockaddr_un> addr_uptr;
public:
    SafeSocket() = delete;
    SafeSocket(const char * socket_path);
    // Delete copy constructors to be  RAII-compliant class
    SafeSocket(SafeSocket& file) = delete;
    SafeSocket& operator=(SafeSocket& file) = delete;
    // Implement move constructors
    SafeSocket(SafeSocket&& file);
    SafeSocket& operator=(SafeSocket&& file);
    ~SafeSocket();

};


class SafeServerSocket : public SafeSocket
{
private:

public:
    SafeServerSocket() = delete;
    SafeServerSocket(const char * socket_path);
    // Delete copy constructors to be  RAII-compliant class
    SafeServerSocket(SafeServerSocket& file) = delete;
    SafeServerSocket& operator=(SafeServerSocket& file) = delete;
    // Implement move constructors
    SafeServerSocket(SafeServerSocket&& file);
    SafeServerSocket& operator=(SafeServerSocket&& file);
    ~SafeServerSocket();
    void serverUp(int maxClientQueue);  // Blind, listen, accept.
    void read(char* Buff, int BuffLength);  
    void unlink();
};

class SafeClientSocket : public SafeSocket
{
private:

public:
    SafeClientSocket() = delete;
    SafeClientSocket(const char * socket_path);
    // Delete copy constructors to be  RAII-compliant class
    SafeClientSocket(SafeClientSocket& file) = delete;
    SafeClientSocket& operator=(SafeClientSocket& file) = delete;
    // Implement move constructors
    SafeClientSocket(SafeClientSocket&& file);
    SafeClientSocket& operator=(SafeClientSocket&& file);
    ~SafeClientSocket();
    void clientConnect();
    void write(char* msg, int msgLength);
};