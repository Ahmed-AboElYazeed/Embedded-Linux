#include <iostream>
#include <utility>
#include <initializer_list>
#include <string>
#include <vector>

//for using open() system call
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

using namespace std;


class fileAction {
    public:
        std::vector<std::pair< std::string,int>> actions;
        /************   constructor   ************/
        fileAction(std::string path)
        {
            cout << "constructor\n";
            // *fd_value = open(path.c_str(),O_WRONLY);
            // fd_value=1;
        }

        /************ class functions ************/
        void registerAction(vector< std::pair< std::string,int> > request )
        {
            cout << "register\n";
            actions=request;
        }

        void excuteAction(void)
        {
            
            while (!actions.empty())
            {
                auto action = actions.back();   // get last action
                actions.pop_back();
                if (action.first == "write")
                {
                    // write(*fd_value,"1", 1);
                    cout << "simulating write\n";
                }
                else if (actions.end().base()->first == "read")
                {
                    // read(*fd_value,(void*)&actions.end().base()->first, actions.end().base()->second);
                    cout << "simulating read\n";
                }
                else if (actions.end().base()->first == "close")
                {
                    // close(*fd_value); 
                    cout << "simulating close\n";
                }
            }
        }

        // void capslockON()
        // {
        //     write(fd_value,"1", 1);
        // }

        /************ copy constructor ************/
        fileAction(fileAction& obj)
        {
            cout << "copy constructor\n";
            fd_value=obj.fd_value;
            obj.fd_value=nullptr;
        }
        /************    destructor    ************/
        ~fileAction()
        {
            cout << "deconstructor\n";
            // close(*fd_value);
            fd_value=0;
        }
    private:
        //std::string path;
        int *fd_value;
        //int*& fdRef;
};


fileAction fun (fileAction& obj)
{
    return obj;
}

int main()
{
    fileAction _fileAction("/sys/class/leds/input2::capslock/brightness");
    _fileAction.registerAction({{"close", 1}, {"read", 1}, {"write",1}});
    _fileAction.excuteAction();
    
    fileAction _fileAction2= fun(_fileAction);
    return 0;
}