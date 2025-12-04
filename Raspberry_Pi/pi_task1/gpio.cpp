/*

sudo echo 538  > /sys/class/gpio/export
    // create: gpio538

sudo echo "out" > /sys/class/gpio/gpio538/direction 
    // set direction ("in", "out")

sudo echo 1 > /sys/class/gpio/gpio538/value 
    //write value (0, 1)

*/

// for testing only
#include <iostream>

//
#include <string>

//for using open() system call
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//for using read() & close() system call
#include <unistd.h>

//for checking if folder exist
#include <filesystem>
#include <iostream>

// inline bool folderExists(const std::string& path) {
//     return std::filesystem::is_directory(path);
// }

namespace COTS 
{
    namespace MCAL
    {
        namespace GPIO
        {
            //gpio.h
            typedef enum{
                IN=0,
                INPUT=0,
                OUT=1,
                OUTPUT=1
            }PinDir_enu_t;

            // gpio.c
            class gpio{
            private:
                uint32_t export_fd = 0;
                uint32_t dir_fd = 0;
                uint32_t val_fd = 0;
            public:
                // constructor
                gpio(uint32_t pin)
                {
                    uint8_t timeout=0;
                    auto pin_str = std::to_string(pin+512);  // the gpio pins are shifted by 512.
                    auto gpioPinFolder = "gpio" + pin_str;
                    auto gpioDirFile = "/sys/class/gpio/"+gpioPinFolder +"/direction";
                    auto gpioValFile = "/sys/class/gpio/"+gpioPinFolder +"/value";
                    
                    std::cout << "<< 3 constructor\n";
                    export_fd = open("/sys/class/gpio/export",O_WRONLY);
                    if(export_fd < 0)
                    {
                        perror("the file did not open");
                        return;
                    }
                    std::cout << export_fd <<"fd created \n";
                    write(export_fd, (pin_str.c_str()), (pin_str.length()));
                    // while (folderExists(gpioPinFolder)!= 1 && (timeout++)<=50)
                    // {
                    //     std::cout << "waiting for gpio file creation\n";
                    //     //std::cout << timeout <<"\n";
                    // }
                    std::cout << " file should be created \n";
                    dir_fd = open(gpioDirFile.c_str(),O_WRONLY);
                    if(dir_fd < 0)
                    {
                        perror("the file did not open");
                        return;
                    }
                    val_fd = open(gpioValFile.c_str(),O_RDWR);
                    if(val_fd < 0)
                    {
                        perror("the file did not open");
                        return;
                    }
                    std::cout << "<< constructor  end \n";
                }
                // copy constructor
                gpio(gpio& obj) = delete;
                // move constructor
                gpio(gpio&& obj) = delete;
                // copy assigment operator
                gpio operator=(gpio& other) = delete;   // only for now
                // move assigment operator
                gpio operator=(gpio&& other) = delete;
                // destructor
                ~gpio()
                {
                    std::cout << ">> deconstructor\n";
                    close(export_fd);
                    close(dir_fd);
                    close(val_fd);
                    std::cout << ">> deconstructor end \n";
                }
                    /**** functions ****/
                void initPin(PinDir_enu_t dir)
                {
                    std::cout << "initPin\n";
                    if (dir==IN)
                    {
                        write(dir_fd,"in", 2);
                        std::cout << "dir is in\n";
                    }
                    else if (dir==OUT)
                    {
                        write(dir_fd,"out", 3);
                        std::cout << "dire is out\n";
                    }
                }
                void setPin()
                {
                    std::cout << "setPin\n";
                    write(val_fd,"1", 1);
                }
                void clearPin()
                {
                    std::cout << "clearPin\n";
                    write(val_fd,"0", 1);
                }
                // void togglePin() //not now...
                // {
                //     std::cout << "togglePin\n";
                // }
                uint8_t readPin()
                {
                    std::cout << "readPin\n";
                    char pinValue[1];
                    read(val_fd,pinValue, 1);
                    return (uint8_t)*pinValue;
                }
            };
            
            // gpio::gpio(/* args */)
            // {
            // }
        }
    }
}





/*
// source:  https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exists-using-standard-c-c11-14-17-c
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>

inline bool exists_test0 (const std::string& name) {
    ifstream f(name.c_str());
    return f.good();
}

inline bool exists_test1 (const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }   
}

inline bool exists_test2 (const std::string& name) {
    return ( access( name.c_str(), F_OK ) != -1 );
}

inline bool exists_test3 (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}
*/