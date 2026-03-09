
//for using shget
#include <sys/ipc.h>
#include <sys/shm.h>


//for using shmat
#include <sys/types.h>
#include <sys/shm.h>

//for NULL, dprintf
#include <stdio.h>

//for using sting functons
#include <string.h>

// the needed arguments
#define MEMKEY 0X1234u
#define SIZE   100u


int main ()
{
    // shmget - allocates a System V shared memory segment
    int shmkey = shmget(MEMKEY, SIZE, IPC_CREAT | 0666);

    // shmat, shmdt - System V shared memory operations
    char *ptr = shmat(shmkey, NULL, 0);  //0 for read/write access.
    
    //code for the second process "reading"
    printf("process 1: %s\n", ptr);

    //responding
    strcpy(ptr, "yes, it is a Super Accurate Fact");

    //the process die and the shared memory still located 
    // as we didnot deattach it 
    // so we dont need the while(1)
    //while(1);
}