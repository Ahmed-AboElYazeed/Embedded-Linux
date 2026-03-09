#include <iostream>
#include <string>
#include <random>
//open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//close
#include <unistd.h>

int main ()
{
    int fd = open("/home/zee/dataLogging/temperature/sensorSimulatorFile.txt", O_WRONLY);  //O_RDONLY, O_WRONLY, or O_RDWR.
    std::string buf;
    std::random_device rd;
    while(1)
    {
        buf = std::to_string(rd());
        buf.insert(2,".");
        std::cout << buf <<std::endl;

        // Seek to beginning and truncate
        lseek(fd, 0, SEEK_SET);
        ftruncate(fd, 0);

        write(fd, buf.c_str(), buf.length());
        for (volatile uint64_t i =0; i<4000000000; i++)
        {
            // wait
        }
    }
}


// #include <fstream>

// int main ()
// {
//    std::ofstream ofs("/home/zee/dataLogging/temperature/sensorSimulatorFile.txt", std::ofstream::trunc);

//    ofs << "lorem ipsum";
//     std::string buf;
//     std::random_device rd;
//     while(1)
//     {
//         buf = std::to_string(rd());
//         buf.insert(2,".");
//         std::cout << buf <<std::endl;
//         ofs << buf.c_str();
//         for (volatile uint64_t i =0; i<900000000; i++)
//         {
//             // wait
//         }
//     }
//    ofs.close();
// }