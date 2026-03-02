#include "Logging&Telemetry/SafeSocket.hpp"
#include <stdexcept>
#include <cstring>      // memset, strerror
#include <cerrno>
#include <unistd.h>     // close(), unlink()

// ══════════════════════════════════════════════════════════
//  Helper — build a human-readable error string
// ══════════════════════════════════════════════════════════
static std::string sys_err(const std::string& ctx)
{
    return ctx + ": " + std::strerror(errno);
}

// ══════════════════════════════════════════════════════════
//  SafeSocket  (base)
// ══════════════════════════════════════════════════════════
SafeSocket::SafeSocket(const std::string& path)
    : socket_path(path),
      addr_uptr(std::make_unique<struct sockaddr_un>())
{
    std::memset(addr_uptr.get(), 0, sizeof(struct sockaddr_un));
    addr_uptr->sun_family = AF_UNIX;

    if (path.size() >= sizeof(addr_uptr->sun_path)) {
        throw std::invalid_argument(
            "Socket path too long (max " +
            std::to_string(sizeof(addr_uptr->sun_path) - 1) + " chars)");
    }

    std::strncpy(addr_uptr->sun_path, path.c_str(), sizeof(addr_uptr->sun_path) - 1);
}

SafeSocket::SafeSocket(SafeSocket&& other) noexcept
    : socket_path(std::move(other.socket_path)),
      addr_uptr(std::move(other.addr_uptr))
{}

SafeSocket& SafeSocket::operator=(SafeSocket&& other) noexcept
{
    if (this != &other) {
        socket_path = std::move(other.socket_path);
        addr_uptr   = std::move(other.addr_uptr);
    }
    return *this;
}

// ══════════════════════════════════════════════════════════
//  SafeServerSocket
// ══════════════════════════════════════════════════════════
SafeServerSocket::SafeServerSocket(const std::string& path)
    : SafeSocket(path)
{
    server_fd = ::socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        throw std::runtime_error(sys_err("SafeServerSocket: socket()"));
    }
}

SafeServerSocket::SafeServerSocket(SafeServerSocket&& other) noexcept
    : SafeSocket(std::move(other)),
      server_fd(other.server_fd),
      client_fd(other.client_fd)
{
    other.server_fd = -1;
    other.client_fd = -1;
}

SafeServerSocket& SafeServerSocket::operator=(SafeServerSocket&& other) noexcept
{
    if (this != &other) {
        if (client_fd != -1) { ::close(client_fd); }
        if (server_fd != -1) { ::close(server_fd); }

        SafeSocket::operator=(std::move(other));
        server_fd = other.server_fd;
        client_fd = other.client_fd;
        other.server_fd = -1;
        other.client_fd = -1;
    }
    return *this;
}

SafeServerSocket::~SafeServerSocket()
{
    if (client_fd != -1) {
        ::close(client_fd);
        client_fd = -1;
    }
    if (server_fd != -1) {
        ::close(server_fd);
        server_fd = -1;
    }
}

// ──────────────────────────────────────────────────────────
void SafeServerSocket::bind_and_listen(int max_client_queue)
{
    if (server_fd == -1) {
        throw std::logic_error("bind_and_listen: socket not initialised");
    }

    // Remove stale socket file if it exists
    ::unlink(socket_path.c_str());

    if (::bind(server_fd,
               reinterpret_cast<struct sockaddr*>(addr_uptr.get()),
               sizeof(struct sockaddr_un)) == -1) {
        throw std::runtime_error(sys_err("bind_and_listen: bind()"));
    }

    if (::listen(server_fd, max_client_queue) == -1) {
        throw std::runtime_error(sys_err("bind_and_listen: listen()"));
    }
}

// ──────────────────────────────────────────────────────────
void SafeServerSocket::accept_client()
{
    if (server_fd == -1) {
        throw std::logic_error("accept_client: not listening (call bind_and_listen first)");
    }

    // Close any previously accepted client before accepting a new one
    if (client_fd != -1) {
        ::close(client_fd);
        client_fd = -1;
    }

    client_fd = ::accept(server_fd, nullptr, nullptr);
    if (client_fd == -1) {
        throw std::runtime_error(sys_err("accept_client: accept()"));
    }
}

// ──────────────────────────────────────────────────────────
ssize_t SafeServerSocket::read_msg(char* buff, size_t buff_len)
{
    if (client_fd == -1) {
        throw std::logic_error("read_msg: no client connected (call accept_client first)");
    }
    ssize_t bytes = ::read(client_fd, buff, buff_len);
    if (bytes == -1) {
        throw std::runtime_error(sys_err("read_msg: read()"));
    }
    return bytes;   // 0 means peer closed connection
}

// ──────────────────────────────────────────────────────────
ssize_t SafeServerSocket::write_msg(const char* msg, size_t msg_len)
{
    if (client_fd == -1) {
        throw std::logic_error("write_msg: no client connected (call accept_client first)");
    }
    ssize_t bytes = ::write(client_fd, msg, msg_len);
    if (bytes == -1) {
        throw std::runtime_error(sys_err("write_msg: write()"));
    }
    return bytes;
}

// ──────────────────────────────────────────────────────────
void SafeServerSocket::unlink_path()
{
    if (::unlink(socket_path.c_str()) == -1 && errno != ENOENT) {
        throw std::runtime_error(sys_err("unlink_path: unlink()"));
    }
}

// ══════════════════════════════════════════════════════════
//  SafeClientSocket
// ══════════════════════════════════════════════════════════
SafeClientSocket::SafeClientSocket(const std::string& path)
    : SafeSocket(path)
{
    client_fd = ::socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_fd == -1) {
        throw std::runtime_error(sys_err("SafeClientSocket: socket()"));
    }
}

SafeClientSocket::SafeClientSocket(SafeClientSocket&& other) noexcept
    : SafeSocket(std::move(other)),
      client_fd(other.client_fd)
{
    other.client_fd = -1;
}

SafeClientSocket& SafeClientSocket::operator=(SafeClientSocket&& other) noexcept
{
    if (this != &other) {
        if (client_fd != -1) { ::close(client_fd); }

        SafeSocket::operator=(std::move(other));
        client_fd = other.client_fd;
        other.client_fd = -1;
    }
    return *this;
}

SafeClientSocket::~SafeClientSocket()
{
    if (client_fd != -1) {
        ::close(client_fd);
        client_fd = -1;
    }
}

// ──────────────────────────────────────────────────────────
void SafeClientSocket::connect_to_server()
{
    if (client_fd == -1) {
        throw std::logic_error("connect_to_server: socket not initialised");
    }

    if (::connect(client_fd,
                  reinterpret_cast<struct sockaddr*>(addr_uptr.get()),
                  sizeof(struct sockaddr_un)) == -1) {
        throw std::runtime_error(sys_err("connect_to_server: connect()"));
    }
}

// ──────────────────────────────────────────────────────────
ssize_t SafeClientSocket::read_msg(char* buff, size_t buff_len)
{
    if (client_fd == -1) {
        throw std::logic_error("read_msg: not connected (call connect_to_server first)");
    }
    ssize_t bytes = ::read(client_fd, buff, buff_len);
    if (bytes == -1) {
        throw std::runtime_error(sys_err("read_msg: read()"));
    }
    return bytes;
}

// ──────────────────────────────────────────────────────────
ssize_t SafeClientSocket::write_msg(const char* msg, size_t msg_len)
{
    if (client_fd == -1) {
        throw std::logic_error("write_msg: not connected (call connect_to_server first)");
    }
    ssize_t bytes = ::write(client_fd, msg, msg_len);
    if (bytes == -1) {
        throw std::runtime_error(sys_err("write_msg: write()"));
    }
    return bytes;
}