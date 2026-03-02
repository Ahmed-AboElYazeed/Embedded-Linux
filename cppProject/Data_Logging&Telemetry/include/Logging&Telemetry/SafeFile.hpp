#pragma once

#include <string>
#include <memory>
// for open()
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// for write() and close()
#include <unistd.h>

class SafeFile
{
private:
    std::unique_ptr<int> fdPtr = std::make_unique<int>(-1);
public:
    // Constructor
    SafeFile() = delete;
    SafeFile(const char*filePath);
    // Delete copy constructors to be  RAII-compliant class
    SafeFile(SafeFile& file) = delete;
    SafeFile& operator=(SafeFile& file) = delete;
    // Implement move constructors
    SafeFile(SafeFile&& file);
    SafeFile& operator=(SafeFile&& file);
    // Destructor
    ~SafeFile();

    void write(const std::string& text) const;
    void read(std::string& buffer, size_t numBytes);
    void operator<<(const std::string& text) const;
    void operator>>(std::string& buffer) const;
};