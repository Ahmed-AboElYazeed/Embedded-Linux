#include "Logging&Telemetry/SafeFile.hpp"

SafeFile::SafeFile(const char*filePath)
{
    // file location start from the project root directory (the CMakeLists.txt file location)
    *fdPtr = open(filePath, O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);
    if (*fdPtr == -1)
    {
        perror("Error in opening Log file");
    }
}


SafeFile::SafeFile(SafeFile&& file)
{
    this->fdPtr = std::move(file.fdPtr);    // move the ownership of the file descriptor from the source object to the current object
    file.fdPtr = std::make_unique<int>(-1);   // reset the source object's file descriptor to -1 to indicate that it no longer owns any file
}
SafeFile& SafeFile::operator=(SafeFile&& file)
{
    if (this != &file)   // check for self-assignment
    {
        delete this->fdPtr.release();   // release the current file descriptor and close the file if it's open
        this->fdPtr = std::move(file.fdPtr);    // move the ownership of the file descriptor from the source object to the current object
        file.fdPtr = std::make_unique<int>(-1);   // reset the source object's file descriptor to -1 to indicate that it no longer owns any file
    }
    return *this;
}
/*  C++ note [smart pointers delete]:
> delete this->fdPtr.release();     (correct):

    release() takes ownership away from the unique_ptr, returning the raw pointer (T*) and setting the smart pointer internal pointer to nullptr.
    delete then destroys that raw pointer.
    Result: Safe, manual deletion. No double-free risks.

> delete this->fdPtr;   (Incorrect for Smart Pointers):

    This attempts to delete the std::unique_ptr object itself, not the object it points to.
*/
SafeFile::~SafeFile()
{
    close(*fdPtr);
}


/*************************************************************/
void SafeFile::write(const std::string& text) const
{
    // \/ The leading "::" ensures that the global system function is called
       ::write(*fdPtr, text.c_str(), text.length());
}
void SafeFile::operator<<(const std::string& text) const
{
    // \/ The leading "::" ensures that the global system function is called
       ::write(*fdPtr, text.c_str(), text.length());
}


/*************************************************************/
void SafeFile::read(std::string& buffer, size_t numBytes)
{
    // \/ The leading "::" ensures that the global system function is called
    ::read(*fdPtr, static_cast<void*> (const_cast<char*>(buffer.c_str())), numBytes);
}
void SafeFile::operator>>(std::string& buffer) const
{
    // \/ The leading "::" ensures that the global system function is called
    ::read(*fdPtr, (void*) buffer.c_str(), buffer.length());
                                                                //  ^ ???
}