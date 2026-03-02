#pragma once
#include <string>
#include <memory>
#include <stdexcept>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

// ─────────────────────────────────────────────
//  Base class — owns the socket path + address
//  Does NOT hold a file descriptor (each subclass owns its own fd)
// ─────────────────────────────────────────────
class SafeSocket
{
protected:
    std::string                        socket_path;
    std::unique_ptr<struct sockaddr_un> addr_uptr;

public:
    SafeSocket() = delete;
    explicit SafeSocket(const std::string& socket_path);

    // Non-copyable
    SafeSocket(const SafeSocket&)            = delete;
    SafeSocket& operator=(const SafeSocket&) = delete;

    // Movable
    SafeSocket(SafeSocket&& other) noexcept;
    SafeSocket& operator=(SafeSocket&& other) noexcept;

    virtual ~SafeSocket() = default;

    const std::string& getPath() const { return socket_path; }
};

// ─────────────────────────────────────────────
//  Server-side socket
//  Lifecycle: construct → bind_and_listen() → accept_client() → read/write → ~
// ─────────────────────────────────────────────
class SafeServerSocket : public SafeSocket
{
private:
    int server_fd  = -1;   // listening socket
    int client_fd  = -1;   // accepted connection

public:
    SafeServerSocket() = delete;
    explicit SafeServerSocket(const std::string& socket_path);

    SafeServerSocket(const SafeServerSocket&)            = delete;
    SafeServerSocket& operator=(const SafeServerSocket&) = delete;

    SafeServerSocket(SafeServerSocket&& other) noexcept;
    SafeServerSocket& operator=(SafeServerSocket&& other) noexcept;

    ~SafeServerSocket() override;

    // Bind and start listening (call once)
    void bind_and_listen(int max_client_queue = 5);

    // Block until a client connects; stores accepted fd internally
    void accept_client();

    // Read from accepted client; returns bytes read (0 = peer closed, -1 = error)
    ssize_t read_msg(char* buff, size_t buff_len);

    // Write to accepted client; returns bytes written (-1 = error)
    ssize_t write_msg(const char* msg, size_t msg_len);

    // Remove the socket file from the filesystem
    void unlink_path();

    bool has_client() const { return client_fd != -1; }
    bool is_listening() const { return server_fd != -1; }
};

// ─────────────────────────────────────────────
//  Client-side socket
//  Lifecycle: construct → connect() → read/write → ~
// ─────────────────────────────────────────────
class SafeClientSocket : public SafeSocket
{
private:
    int client_fd = -1;

public:
    SafeClientSocket() = delete;
    explicit SafeClientSocket(const std::string& socket_path);

    SafeClientSocket(const SafeClientSocket&)            = delete;
    SafeClientSocket& operator=(const SafeClientSocket&) = delete;

    SafeClientSocket(SafeClientSocket&& other) noexcept;
    SafeClientSocket& operator=(SafeClientSocket&& other) noexcept;

    ~SafeClientSocket() override;

    // Connect to the server at socket_path
    void connect_to_server();

    // Read from server; returns bytes read (0 = server closed, -1 = error)
    ssize_t read_msg(char* buff, size_t buff_len);

    // Write to server; returns bytes written (-1 = error)
    ssize_t write_msg(const char* msg, size_t msg_len);

    bool is_connected() const { return client_fd != -1; }
};