#include "Logging&Telemetry/SafeSocket.hpp"


SafeSocket::SafeSocket(const char * socket_file_path)
{
    if(socket_file_path != nullptr)
    {
        socket_path = socket_file_path;

        // create a socket
        *server_fd_uptr = socket(AF_UNIX, SOCK_STREAM, 0);
        if (*server_fd_uptr == -1) {
            std::cout << "socket error\n";
            // perror("socket");
            // exit(EXIT_FAILURE);
        }

        // For portability clear the whole structure, since some
        memset(addr_uptr.get(), 0, sizeof(*addr_uptr));
    
        // Connect socket to socket address.
        addr_uptr->sun_family = AF_UNIX;
        strncpy(addr_uptr->sun_path, socket_path, sizeof(addr_uptr->sun_path) - 1);
    }
    else
    {
        std::cout << "A nullptr socket_file_path error\n";
    }

}

SafeSocket::~SafeSocket()
{

}
