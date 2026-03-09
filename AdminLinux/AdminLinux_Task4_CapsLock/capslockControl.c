
//for using open() system call
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//for using dprintf()
#include <stdio.h>

//for using close()
#include <unistd.h>

//for comparing strings
#include <string.h>


int main (int argc, char * argv[])
{
    // step 1 --> OPEN the file    /* man 2 open */          // using \\ instead of \ in the path
    int fd_value = open("/sys/class/leds/input2::capslock/brightness",O_RDWR);

    // step 2 --> writing logic     /* man dprintf */ 
    if (strncmp(argv[1] ,"1" , 1) == 0u) //if the input string first character equal "1" --> capslock >> ON 
    {
        dprintf(1,"openning the capslock led  %d\n", fd_value); //for depugging
        dprintf(fd_value, "1"); //for writing on the file discriptor //not just for printing
    }
    else //any thing close the capslock led 
    {
        dprintf(1,"closing the capslock led %d \n ",strncmp(argv[1] ,"1 " , 1) ); //for depugging
        dprintf(fd_value,"0"); //for writing on the file discriptor //not just for printing
    }

    while(1);
    // step 3 --> close the file   or   deleting the file discriptor    /* man close */ 
    close(fd_value);


    return 0;
}