

//for using shget
#include <sys/ipc.h>
#include <sys/shm.h>


//for using shmat
#include <sys/types.h>
#include <sys/shm.h>

//for NULL
#include <stdio.h>

//for using sting functons
#include <string.h>

// the needed arguments
#define MEMKEY 0X1234u
#define SIZE   100u


int main ()
{
    // shmget - allocates a System V shared memory segment
    int shmid = shmget(MEMKEY, SIZE, IPC_CREAT | 0666);

    // shmat, shmdt - System V shared memory operations
    char *ptr = shmat(shmid, NULL, 0);  //0 for read/write access.
    
    //for debugging the shared memory creation
    printf("Shared memory segment created with ID: %d\n", shmid);

    //code for the first process "writing"
    strcpy(ptr, "Eng Fady is the linux king\n");

    //for listning
    while(strcmp("yes, it is a Super Accurate Fact", ptr) != 0);
    
    printf("process 2: %s\n", ptr);
}




/* while (1)
    {
        //reading 
        read memory
        if new message 
        print the message
        if same as i sent 
        leave it as it is
        
        char * last
        for (int i =1 ; i >3 ; i++ )
        {
            if 
        }
        
        //writing
        scanf 
        if has \n 
        write the scanned on the memory
    }
*/