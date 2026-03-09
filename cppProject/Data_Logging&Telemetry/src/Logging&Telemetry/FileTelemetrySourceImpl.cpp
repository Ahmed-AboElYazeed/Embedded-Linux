#include "Logging&Telemetry/FileTelemetrySourceImpl.hpp"
#include <iostream>
#include <cstring>

FileTelemetrySourceImpl::FileTelemetrySourceImpl(const std::string& path)
    : source_file(path)
{}

bool FileTelemetrySourceImpl::openSource()
{
    if (!source_file.open_file()) {
        std::cerr << "[FileTelemetrySourceImpl] openSource() failed\n";
        return false;
    }
    return true;
}

bool FileTelemetrySourceImpl::readSource(std::string& out)
{
    if (!source_file.is_open()) {
        std::cerr << "[FileTelemetrySourceImpl] readSource() called before openSource()\n";
        return false;
    }

    /*
     * NOTE — why we rewind before every read:
     *   WRONG:  read from current fd position each call
     *   RIGHT:  lseek to start, then scan to the last line
     *
     *   The shell script keeps appending lines. Without rewinding, the fd
     *   position stays at EOF and every read returns 0 bytes. We rewind
     *   and scan forward to find the last newline-terminated value.
     */
    if (!source_file.rewind()) {
        std::cerr << "[FileTelemetrySourceImpl] rewind() failed\n";
        return false;
    }

    char    buffer[4096];
    ssize_t bytes = source_file.read_file(buffer, sizeof(buffer) - 1);

    if (bytes <= 0) {
        std::cerr << "[FileTelemetrySourceImpl] read_file() returned " << bytes << "\n";
        return false;
    }

    buffer[bytes] = '\0';

    /*
     * NOTE — why we search for the last newline instead of the first:
     *   WRONG:  returning the first token — that is always the oldest entry
     *   RIGHT:  walk backward from the end to find the last complete line
     *
     *   The file grows over time. The latest reading is always at the end.
     *   If the last character is '\n' we skip it, then find the newline
     *   before the last value to extract just that line.
     */
    int end = bytes - 1;
    while (end >= 0 && buffer[end] == '\n') 
    { --end; }

    int start = end;
    while (start > 0 && buffer[start - 1] != '\n') 
    { --start; }

    if (start > end) {
        std::cerr << "[FileTelemetrySourceImpl] no valid line found in file\n";
        return false;
    }

    out = std::string(buffer + start, end - start + 1);
    return true;
}