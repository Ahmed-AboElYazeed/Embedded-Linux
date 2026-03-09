#include <iostream>
#include <utility>
#include <initializer_list>
#include <string>

//for using open() system call
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>



class capslock {
    public:
        std::initializer_list< std::pair< std::string,int > > vector;

        /************   constructor   ************/
        capslock(std::string path)
        {
            //path = obj;
            fd_value = open(path.c_str(),O_WRONLY);
        }

        /************ class functions ************/
        void registerAction( std::initializer_list< std::pair< std::string,int > > request )
        {
            vector = request;
        }

        void excuteAction(void)
        {
            if (vector.begin()->first =="write")
            {
                write(fd_value,"1", 1);
            }
            else if (vector.begin()->first =="read")
            {
                read(fd_value,(void*)&vector.begin()->first, vector.begin()->second);
            }
            else if (vector.begin()->first =="close")
            {
                close(fd_value);
            }
        }

        void capslockON()
        {
            write(fd_value,"1", 1);
        }

        /************ copy constructor ************/
        capslock(capslock& obj)
        {
            obj.fd_value=fd_value;
        }
        /************    destructor    ************/
        ~capslock()
        {
            close(fd_value);
        }
    private:
        //std::string path;
        int fd_value;
        //int*& fdRef;
};


// void fun (capslock obj)
// {
//     return 
// }

int main()
{
    capslock _capslock("/sys/class/leds/input2::capslock/brightness");
    _capslock.capslockON();

    _capslock.registerAction({{"write",1}});
    _capslock.excuteAction();

    _capslock.registerAction({{"read",1}});
    _capslock.excuteAction();

    std::cout << _capslock.vector.begin()->first << std::endl;


}