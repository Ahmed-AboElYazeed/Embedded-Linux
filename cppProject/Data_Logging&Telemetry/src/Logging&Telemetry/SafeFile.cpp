#include "Logging&Telemetry/SafeFile.hpp"
#include <cstring>
#include <cerrno>


SafeFile::SafeFile(const std::string& path)
    : fd(-1), file_path(path)
{
    /*
     * NOTE — why the constructor no longer calls open():
     *
     *   WRONG:  opening the file inside the constructor with O_WRONLY | O_CREAT
     *   RIGHT:  store the path, let open_file() open it explicitly later
     *
     *   FileTelemetrySourceImpl calls openSource() as a separate step.
     *   Opening in the constructor means the file is opened before the
     *   caller is ready, and there is no way to report or recover from
     *   failure without exceptions.
     */
}

SafeFile::SafeFile(SafeFile&& other) noexcept
    : fd(other.fd), file_path(std::move(other.file_path))
{
    other.fd = -1;

    /*
     * NOTE — why we null out the source fd after moving:
     *
     *   WRONG:  leaving other.fd as-is after the move
     *   RIGHT:  set other.fd = -1 immediately
     *
     *   If we don't, both the moved-from and moved-to objects hold the same
     *   fd integer. When the moved-from destructor runs, it calls ::close()
     *   on that fd. The moved-to object now holds a closed (invalid) fd
     *   and all subsequent reads/writes silently fail or hit a recycled fd.
     */
}

SafeFile& SafeFile::operator=(SafeFile&& other) noexcept
{
    if (this != &other) {
        if (fd != -1) {
            ::close(fd);
        }

        /*
         * NOTE — why we do NOT use delete fdPtr.release() here:
         *
         *   WRONG:  delete this->fdPtr.release();
         *   RIGHT:  ::close(fd) then fd = other.fd; other.fd = -1;
         *
         *   File descriptors are not heap memory — you do not delete them.
         *   You close them with ::close(). Calling delete on an int* that
         *   points to a stack or heap int is undefined behaviour and does
         *   not close the underlying kernel file description at all.
         */
        fd        = other.fd;
        file_path = std::move(other.file_path);
        other.fd  = -1;
    }
    return *this;
}

SafeFile::~SafeFile()
{
    if (fd != -1) {
        ::close(fd);
        fd = -1;
    }

    /*
     * NOTE — why we guard with if (fd != -1) before close():
     *
     *   WRONG:  ::close(*fdPtr) unconditionally in destructor
     *   RIGHT:  check fd != -1 first
     *
     *   ::close(-1) is undefined behaviour on some systems and returns
     *   EBADF on others. After a move, the source object's fd is -1 —
     *   without the guard its destructor would attempt to close fd -1.
     */
}

bool SafeFile::open_file()
{
    if (fd != -1) {
        return false;
    }

    fd = ::open(file_path.c_str(), O_RDONLY);
    if (fd == -1) {
        return false;
    }

    return true;
}

ssize_t SafeFile::read_file(char* buff, size_t len)
{
    if (fd == -1) {
        return -1;
    }

    ssize_t bytes = ::read(fd, buff, len);
    if (bytes == -1) {
        return -1;
    }
    return bytes;
}

bool SafeFile::rewind()
{
    if (fd == -1) {
        return false;
    }

    /*
     * NOTE — why lseek instead of re-opening the file:
     *
     *   WRONG:  ::close(fd); fd = ::open(file_path, O_RDONLY);
     *   RIGHT:  ::lseek(fd, 0, SEEK_SET)
     *
     *   Re-opening wastes a syscall and a new fd slot. lseek simply moves
     *   the kernel's position pointer for the already-open fd back to byte 0.
     *   It is also atomic with respect to the fd — no window where fd == -1.
     */
    if (::lseek(fd, 0, SEEK_SET) == -1) {
        return false;
    }

    return true;
}

void SafeFile::write(const std::string& text) const
{
    if (fd == -1) {
        return;
    }
    ::write(fd, text.c_str(), text.length());
}

void SafeFile::operator<<(const std::string& text) const
{
    write(text);
}