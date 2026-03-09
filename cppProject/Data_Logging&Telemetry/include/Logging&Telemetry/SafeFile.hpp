#pragma once
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>

class SafeFile
{
private:
    int         fd        = -1;
    std::string file_path;

    /*
     * NOTE — why int fd instead of std::unique_ptr<int>:
     *
     *   WRONG:  std::unique_ptr<int> fdPtr = std::make_unique<int>(-1);
     *   RIGHT:  int fd = -1;
     *
     *   A file descriptor is a plain integer. unique_ptr<int> heap-allocates
     *   a single int for no benefit, and worse — its destructor calls
     *   operator delete on an int, NOT ::close(). The fd leaks silently.
     *   Use -1 as the POSIX sentinel for "no open file".
     */

public:
    SafeFile() = delete;
    explicit SafeFile(const std::string& path);

    /*
     * NOTE — why copy constructors must take const&:
     *
     *   WRONG:  SafeFile(SafeFile& file) = delete;
     *   RIGHT:  SafeFile(const SafeFile& file) = delete;
     *
     *   Without const, the compiler does not recognise it as the canonical
     *   copy constructor and may silently generate one on its own.
     */
    SafeFile(const SafeFile&)            = delete;
    SafeFile& operator=(const SafeFile&) = delete;

    SafeFile(SafeFile&& other) noexcept;
    SafeFile& operator=(SafeFile&& other) noexcept;

    ~SafeFile();

    // Opens the file for reading only — used by FileTelemetrySourceImpl
    bool open_file();

    // Reads up to len bytes into buff; returns bytes read or -1
    ssize_t read_file(char* buff, size_t len);

    // Seeks back to the beginning of the file
    bool rewind();

    // Writes text to file (must be opened with write flags separately if needed)
    void write(const std::string& text) const;
    void operator<<(const std::string& text) const;

    bool is_open() const { return fd != -1; }
};