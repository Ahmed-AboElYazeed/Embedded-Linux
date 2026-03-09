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
                /*
                gpio(uint8_t pinNumber, uint32_t* pin)
                {
                    for (int i=0; i<pinNumber; i++)
                    {
                        auto pin_str = std::to_string(pin[i]+512);  // the gpio pins are shifted by 512.
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
                }
                */
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
                void writePinValue(int8_t value)
                {
                    std::cout << "writePinValue\n";
                    write(val_fd,(std::to_string(value)).c_str(), 1); 
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


#include <iostream>
#include <string>

/***************** First layer **************/
class stream
{
    private:
        std::string path;
    public:
        stream(std::string& userPath)
        {
            std::cout << "stream constructor \n";
            path = userPath;
        }
        ~stream()
        {
            std::cout << "stream destructor \n";
        }
        virtual void open() = 0;
};

/***************** Second layer *************/
class inStream: virtual public stream
{
    private:
        std::istream& in;    // must be reference (&) {BECAUSE}--> move constructor is implemented & copy constructor is deleted  ::::  [basic_istream(const basic_istream&) = delete;] 
    public:
        inStream(std::istream& userInStream, std::string& userPath) : stream (userPath), in(userInStream)
        {
            std::cout << "inStream constructor \n";
        }
        ~inStream()
        {
            std::cout << "inStream destructor \n";
        }
        void open()
        {
            std::cout<<"open from inStream\n";
        }
        std::istream& operator>>(std::string userString)
        {
            std::cout << "Please write a number between 0 and 9\n";
            if (userString.length() == 1 && static_cast<unsigned char>(userString[0]) >= '0' && static_cast<unsigned char>(userString[0]) <= '9' )
            {
                in >> userString;   //because they did implement operator overload for >> in tje istream class.
            }
            else
            {
                std::cout << "wrong input\n";
            }
            return in;
        }
};

class outStream: virtual public stream
{
    private:
        std::ostream& out;
    public:
        outStream(std::ostream& userOutStream, std::string& userPath): stream(userPath), out(userOutStream){
            std::cout<<"from outStream\n";
        }
        ~outStream(){
            std::cout << "outstrem Desctructor \n";
        }

        std::ostream& operator<<(std::string& outstring)
        {
            /*read from fd the contents and save it in outstring */
            out << outstring;
            return out;
        }
};

/***************** Third layer **************/
class ioStream: public outStream, public inStream 
{
    public:
        ioStream(std::string& userPath, std::istream& userInStream, std::ostream& userOutStream):
                        inStream(userInStream, userPath),
                        outStream(userOutStream, userPath),
                        stream(userPath)
        {
            std::cout << "iostream Constructor \n";
        }
        ~ioStream()
        {
            std::cout << "iostream Destructor \n";
        }
};

class sevenSegment: public outStream, public inStream
{
    private:
        uint32_t pinNumbers [7]={1,2,34};
        COTS::MCAL::GPIO::gpio pinA;
        COTS::MCAL::GPIO::gpio pinB;
        COTS::MCAL::GPIO::gpio pinC;
        COTS::MCAL::GPIO::gpio pinD;
        COTS::MCAL::GPIO::gpio pinE;
        COTS::MCAL::GPIO::gpio pinF;
        COTS::MCAL::GPIO::gpio pinG;
    public:
        sevenSegment(std::string& userPath, std::istream& userInStream, std::ostream& userOutStream):
                        inStream(userInStream, userPath),
                        outStream(userOutStream, userPath),
                        stream(userPath),
                        pinA(pinNumbers[0]), pinB(pinNumbers[1]), pinC(pinNumbers[2]), pinD(pinNumbers[3]), pinE(pinNumbers[4]), pinF(pinNumbers[5]), pinG(pinNumbers[6])
                        // COTS::MCAL::GPIO::gpio::gpio
        {
            std::cout << "sevenSegment Constructor \n";
            pinA.initPin(COTS::MCAL::GPIO::OUT);
            pinB.initPin(COTS::MCAL::GPIO::OUT);
            pinC.initPin(COTS::MCAL::GPIO::OUT);
            pinD.initPin(COTS::MCAL::GPIO::OUT);
            pinE.initPin(COTS::MCAL::GPIO::OUT);
            pinF.initPin(COTS::MCAL::GPIO::OUT);
            pinG.initPin(COTS::MCAL::GPIO::OUT);
        }
        ~sevenSegment()
        {
            std::cout << "sevenSegment Destructor \n";
        }
        void writeNumber(std::string numberStr)
        {
             static const uint8_t segmentMap[10][7] =
            {
                /*A B C D E F G*/
                {1,1,1,1,1,1,0}, // 0
                {0,1,1,0,0,0,0}, // 1
                {1,1,0,1,1,0,1}, // 2
                {1,1,1,1,0,0,1}, // 3
                {0,1,1,0,0,1,1}, // 4
                {1,0,1,1,0,1,1}, // 5
                {1,0,1,1,1,1,1}, // 6
                {1,1,1,0,0,0,0}, // 7
                {1,1,1,1,1,1,1}, // 8
                {1,1,1,1,0,1,1}  // 9
            };

            int digit = numberStr[0] - '0';

            COTS::MCAL::GPIO::gpio* pins[7] =
            {
                &pinA, &pinB, &pinC, &pinD, &pinE, &pinF, &pinG
            };

            for (int i = 0; i < 7; ++i)
            {
                if (segmentMap[digit][i])
                    pins[i]->setPin();
                else
                    pins[i]->clearPin();
            }
        }
};

int main(int argc, char const *argv[])
{
    std::string selector;
    if (argc >= 1)
    {
        if (std::string(argv[0]) == "1")
        {
            selector = "1";
        }
        else if (std::string(argv[0]) == "0")
        {
            selector = "0";
        }
        else
        {
            std::cout << "Please select the output method \n 0: terminal \n 1:seven segment \n";
            std::cin >> selector;
        }
    }
    else
    {
        std::cout << "Please select the output method \n 0: terminal \n 1:seven segment \n";
        std::cin >> selector;
    }

    // If i have time i will make it create a file (but not now) 
    std::string path ="/home/zee/ITI_Files/linux/Embedded-Linux/cppTask5/iofile.txt";

    if (selector == "1")    // use the seven segment
    {
        std::string inputString;
        sevenSegment inout (path, std::cin, std::cout);
        inout >> inputString;
        inout << inputString << std::endl;
    }
    else    // use the teminal
    {
        std::string inputString;
        ioStream inout (path, std::cin, std::cout);
        inout >> inputString;
        inout << inputString << std::endl;
    }
}

