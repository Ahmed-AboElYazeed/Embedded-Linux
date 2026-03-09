#include "Logging&Telemetry/SafeSocket.hpp"
#include <cstring>      // memset, strerror, strncpy
#include <cerrno>
#include <unistd.h>     // close(), unlink(), read(), write()

// ══════════════════════════════════════════════════════════
//  SafeSocket  (base)
// ══════════════════════════════════════════════════════════
void SafeSocket::log_err(const std::string& ctx, const std::string& detail)
{
    std::cerr << "[SafeSocket ERROR] " << ctx;
    if (!detail.empty())
        std::cerr << " | " << detail;
    std::cerr << std::endl;
}

SafeSocket::SafeSocket(const std::string& path)
    : socket_path(path),
      addr_uptr(nullptr)
{
    if (path.empty()) {
        log_err("SafeSocket()", "socket path is empty");
        return;
    }

    auto addr = std::make_unique<struct sockaddr_un>();
    std::memset(addr.get(), 0, sizeof(struct sockaddr_un));
    addr->sun_family = AF_UNIX;

    if (path.size() >= sizeof(addr->sun_path)) {
        log_err("SafeSocket()", "socket path too long: " + path);
        return;
    }

    std::strncpy(addr->sun_path, path.c_str(), sizeof(addr->sun_path) - 1);
    addr_uptr = std::move(addr);
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
    if (!isReady()) {
        log_err("SafeServerSocket()", "base initialisation failed, skipping socket()");
        return;
    }

    server_fd = ::socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        log_err("SafeServerSocket(): socket()", std::strerror(errno));
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
bool SafeServerSocket::bind_and_listen(int max_client_queue)
{
    if (server_fd == -1) {
        log_err("bind_and_listen()", "invalid server_fd — was construction successful?");
        return false;
    }

    // Remove stale socket file if present
    ::unlink(socket_path.c_str());

    if (::bind(server_fd,
               reinterpret_cast<struct sockaddr*>(addr_uptr.get()),
               sizeof(struct sockaddr_un)) == -1) {
        log_err("bind_and_listen(): bind()", std::strerror(errno));
        return false;
    }

    if (::listen(server_fd, max_client_queue) == -1) {
        log_err("bind_and_listen(): listen()", std::strerror(errno));
        return false;
    }

    return true;
}

// ──────────────────────────────────────────────────────────
bool SafeServerSocket::accept_client()
{
    if (server_fd == -1) {
        log_err("accept_client()", "not listening — call bind_and_listen() first");
        return false;
    }

    // Close any previously accepted client
    if (client_fd != -1) {
        ::close(client_fd);
        client_fd = -1;
    }

    client_fd = ::accept(server_fd, nullptr, nullptr);
    if (client_fd == -1) {
        log_err("accept_client(): accept()", std::strerror(errno));
        return false;
    }

    return true;
}

// ──────────────────────────────────────────────────────────
ssize_t SafeServerSocket::read_msg(char* buff, size_t buff_len)
{
    if (client_fd == -1) {
        log_err("read_msg()", "no client connected — call accept_client() first");
        return -1;
    }

    ssize_t bytes = ::read(client_fd, buff, buff_len);
    if (bytes == -1) {
        log_err("read_msg(): read()", std::strerror(errno));
    }
    return bytes;   // 0 = peer closed
}

// ──────────────────────────────────────────────────────────
ssize_t SafeServerSocket::write_msg(const char* msg, size_t msg_len)
{
    if (client_fd == -1) {
        log_err("write_msg()", "no client connected — call accept_client() first");
        return -1;
    }

    ssize_t bytes = ::write(client_fd, msg, msg_len);
    if (bytes == -1) {
        log_err("write_msg(): write()", std::strerror(errno));
    }
    return bytes;
}

// ──────────────────────────────────────────────────────────
bool SafeServerSocket::unlink_path()
{
    if (::unlink(socket_path.c_str()) == -1 && errno != ENOENT) {
        log_err("unlink_path(): unlink()", std::strerror(errno));
        return false;
    }
    return true;
}

// ══════════════════════════════════════════════════════════
//  SafeClientSocket
// ══════════════════════════════════════════════════════════
SafeClientSocket::SafeClientSocket(const std::string& path)
    : SafeSocket(path)
{
    if (!isReady()) {
        log_err("SafeClientSocket()", "base initialisation failed, skipping socket()");
        return;
    }

    client_fd = ::socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_fd == -1) {
        log_err("SafeClientSocket(): socket()", std::strerror(errno));
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
bool SafeClientSocket::connect_to_server()
{
    if (client_fd == -1) {
        log_err("connect_to_server()", "invalid client_fd — was construction successful?");
        return false;
    }

    if (::connect(client_fd,
                  reinterpret_cast<struct sockaddr*>(addr_uptr.get()),
                  sizeof(struct sockaddr_un)) == -1) {
        log_err("connect_to_server(): connect()", std::strerror(errno));
        return false;
    }

    return true;
}

// ──────────────────────────────────────────────────────────
ssize_t SafeClientSocket::read_msg(char* buff, size_t buff_len)
{
    if (client_fd == -1) {
        log_err("read_msg()", "not connected — call connect_to_server() first");
        return -1;
    }

    ssize_t bytes = ::read(client_fd, buff, buff_len);
    if (bytes == -1) {
        log_err("read_msg(): read()", std::strerror(errno));
    }
    return bytes;
}

// ──────────────────────────────────────────────────────────
ssize_t SafeClientSocket::write_msg(const char* msg, size_t msg_len)
{
    if (client_fd == -1) {
        log_err("write_msg()", "not connected — call connect_to_server() first");
        return -1;
    }

    ssize_t bytes = ::write(client_fd, msg, msg_len);
    if (bytes == -1) {
        log_err("write_msg(): write()", std::strerror(errno));
    }
    return bytes;
}