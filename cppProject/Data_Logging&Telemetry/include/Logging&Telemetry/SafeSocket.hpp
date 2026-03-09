#pragma once
#include <string>
#include <memory>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

// ─────────────────────────────────────────────
//  Base class — owns the socket path + address
// ─────────────────────────────────────────────
class SafeSocket
{
protected:
    std::string                         socket_path;
    std::unique_ptr<struct sockaddr_un> addr_uptr;

    // Helper: print formatted error to stderr
    static void log_err(const std::string& ctx, const std::string& detail = "");

public:
    SafeSocket() = delete;
    explicit SafeSocket(const std::string& socket_path);

    SafeSocket(const SafeSocket&)            = delete;
    SafeSocket& operator=(const SafeSocket&) = delete;

    SafeSocket(SafeSocket&& other) noexcept;
    SafeSocket& operator=(SafeSocket&& other) noexcept;

    virtual ~SafeSocket() = default;

    const std::string& getPath() const { return socket_path; }
    bool               isReady() const { return addr_uptr != nullptr; }
};

// ─────────────────────────────────────────────
//  Server-side socket
//  Lifecycle: construct → bind_and_listen() → accept_client() → read/write → ~
// ─────────────────────────────────────────────
class SafeServerSocket : public SafeSocket
{
private:
    int server_fd = -1;
    int client_fd = -1;

public:
    SafeServerSocket() = delete;
    explicit SafeServerSocket(const std::string& socket_path);

    SafeServerSocket(const SafeServerSocket&)            = delete;
    SafeServerSocket& operator=(const SafeServerSocket&) = delete;

    SafeServerSocket(SafeServerSocket&& other) noexcept;
    SafeServerSocket& operator=(SafeServerSocket&& other) noexcept;

    ~SafeServerSocket() override;

    // Returns true on success
    bool bind_and_listen(int max_client_queue = 5);

    // Returns true on success
    bool accept_client();

    // Returns bytes read, 0 = peer closed, -1 = error
    ssize_t read_msg(char* buff, size_t buff_len);

    // Returns bytes written, -1 = error
    ssize_t write_msg(const char* msg, size_t msg_len);

    // Returns true on success
    bool unlink_path();

    bool has_client()   const { return client_fd != -1; }
    bool is_listening() const { return server_fd != -1; }
};

// ─────────────────────────────────────────────
//  Client-side socket
//  Lifecycle: construct → connect_to_server() → read/write → ~
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

    // Returns true on success
    bool connect_to_server();

    // Returns bytes read, 0 = server closed, -1 = error
    ssize_t read_msg(char* buff, size_t buff_len);

    // Returns bytes written, -1 = error
    ssize_t write_msg(const char* msg, size_t msg_len);

    bool is_connected() const { return client_fd != -1; }
};