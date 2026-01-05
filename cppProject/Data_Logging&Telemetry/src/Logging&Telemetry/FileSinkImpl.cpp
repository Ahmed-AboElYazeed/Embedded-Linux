#include "Logging&Telemetry/FileSinkImpl.hpp"
// POSIX headers
#include <sys/types.h>  // for open
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>    // for close, write

FileSinkImpl::FileSinkImpl(/* args */)
{
    // file location start from the project root directory (the CMakeLists.txt file location)
    *fdPtr = open("logfile.txt", O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);
    if (*fdPtr == -1)
    {
        perror("Error in opening Log file");
    }
}
FileSinkImpl::~FileSinkImpl()
{
    close(*fdPtr);
}

void FileSinkImpl::write (const LogMessage& msg)
{
    // \/ The leading :: ensures that the global system function is called
       ::write(*fdPtr, msg.getMessage().c_str(), msg.getMessage().length());
}
void FileSinkImpl::operator<<(const LogMessage& msg)
{
        // \/ The leading :: ensures that the global system function is called
       ::write(*fdPtr, msg.getMessage().c_str(), msg.getMessage().length());
}