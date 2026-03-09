#include <iostream>
#include <memory>
namespace COTS 
{
    namespace APP
    {
        namespace TEMP_MESUR
        {
            //open
            #include <sys/types.h>
            #include <sys/stat.h>
            #include <fcntl.h>
            //close
            #include <unistd.h>
            //string
            #include <string>
            
            class tempratureData{
                private:
                    float temperature;
                public:
                    void setTemperature(float value)
                    {
                        temperature = value;
                    }
                    float getTemperature() const
                    {
                        return temperature;
                    }
            };
            class LMsensor{
                private:
                    std::unique_ptr<int> fd_Uptr;
                    std::string buf;
                public:
                    LMsensor()
                    {
                        fd_Uptr = std::make_unique<int>(1);
                        *fd_Uptr = open("/home/zee/dataLogging/temperature/sensorSimulatorFile.txt", O_RDONLY);  //O_RDONLY, O_WRONLY, or O_RDWR.
                    }
                    ~LMsensor()
                    {   
                        close(*fd_Uptr);
                        // delete fd_Uptr.get();   // the unique ptr distructor will handel this 
                    }
                    void readTemperature(std::shared_ptr<tempratureData> temp)
                    {
                        lseek(*fd_Uptr, 0, SEEK_SET);  // ← ADD THIS: Reset to beginning

                        char buffer[32] = {0};  // Use a char array
                        ssize_t bytes_read = read(*fd_Uptr, buffer, sizeof(buffer) - 1);
                        
                        if (bytes_read > 0) 
                        {
                            buffer[bytes_read] = '\0';  // Ensure null termination
                            buf = buffer;  // Copy to string if needed
                            temp->setTemperature(std::stof(buf)); 
                        }
                        // read(*fd_Uptr,&buf[0],4);
                        // temp->setTemperature(std::stof(buf)); 
                    }
            };
            class logger{
                private:
                    // std::unique_ptr<int> fd_Uptr {std::make_unique<int> (1)}; 
                    std::unique_ptr<int> fd_Uptr {new int (1)};  
                    std::string buf;
                public:
                    logger()
                    {
                        *fd_Uptr = open("/home/zee/dataLogging/temperature/temp.log", O_WRONLY | O_CREAT | O_APPEND, 0644); //O_RDONLY, O_WRONLY, or O_RDWR.
                    }
                    ~logger()
                    {
                        close(*fd_Uptr);
                        // delete fd_Uptr.get();   // the unique ptr distructor will handel this (even if you used "new" instead of "make_unique")
                    }
                    void logTemp(std::shared_ptr<tempratureData> tempData_Sptr)
                    {
                        buf = "temp = \t" + std::to_string(tempData_Sptr->getTemperature()) + "\n";
                        write(*fd_Uptr, buf.c_str(), buf.length());
                    }
            };
            class display{
                public:
                    display(){}
                    ~display(){}
                    display (display& obj) = delete;
                    display (display&& obj) = delete;
                    void displayTemp(std::shared_ptr<tempratureData> tempData_Sptr)
                    {
                        std::cout << "current temperature = "<< tempData_Sptr->getTemperature() << std::endl;
                    }
            };
        }
    }
}

using namespace COTS::APP::TEMP_MESUR;

int main()
{
    //for (int k =0; k<10; k++)    
    while(1)
    {
        {       /*  to reopen the file each time to read the new value */
            std::unique_ptr<LMsensor> sense = std::make_unique<LMsensor>(); // instead of using new
            std::shared_ptr<tempratureData> tempratureData_1_Sptr = std::make_shared<tempratureData>();
            sense->readTemperature(tempratureData_1_Sptr);
            //or 
            // tempratureData _tempratureData_2_obj;
            // (*sense).readTemperature(std::make_shared<tempratureData> (_tempratureData_2_obj));
            display disp;
            logger log;
            disp.displayTemp(tempratureData_1_Sptr);
            log.logTemp(tempratureData_1_Sptr);        
        }
        for (volatile uint64_t i =0; i<9000000000; i++)
        {
            // wait
        }
    }

}