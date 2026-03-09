#include "Logging&Telemetry/SocketTelemetrySourceImpl.hpp"
#include <iostream>

SocketTelemetrySourceImpl::SocketTelemetrySourceImpl(const std::string& path)
    : source_socket(path)
{}

bool SocketTelemetrySourceImpl::openSource()
{
    if (!source_socket.connect_to_server()) {
        std::cerr << "[SocketTelemetrySourceImpl] openSource(): connect failed\n";
        return false;
    }
    return true;
}

bool SocketTelemetrySourceImpl::readSource(std::string& out)
{
    if (!source_socket.is_connected()) {
        std::cerr << "[SocketTelemetrySourceImpl] readSource() called before openSource()\n";
        return false;
    }

    char    buffer[256];
    ssize_t bytes = source_socket.read_msg(buffer, sizeof(buffer) - 1);

    /*
     * NOTE — why 0 is treated as a failure here, not just -1:
     *   WRONG:  if (bytes < 0) return false;
     *   RIGHT:  if (bytes <= 0) return false;
     *
     *   bytes == 0 means the server closed the connection. Treating it as
     *   success would store an empty string in `out` and the caller would
     *   silently process a blank telemetry reading as valid data.
     */
    if (bytes <= 0) {
        std::cerr << "[SocketTelemetrySourceImpl] readSource(): connection closed or error\n";
        return false;
    }

    buffer[bytes] = '\0';
    out = std::string(buffer, bytes);

    /*
     * NOTE — why trim trailing newline from socket data:
     *   WRONG:  storing raw buffer directly — may contain \n from the sender's writeln
     *   RIGHT:  strip trailing \n or \r\n so `out` holds only the value
     *
     *   Many server implementations append a newline to each message.
     *   Leaving it in causes string comparisons and float parsing to fail
     *   silently downstream.
     */
    while (!out.empty() && (out.back() == '\n' || out.back() == '\r')) {
        out.pop_back();
    }

    return true;
}