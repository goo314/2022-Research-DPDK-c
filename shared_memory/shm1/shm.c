#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    key_t k = ftok(".", 'b'); // create key
    int shm_id = shmget(k, sizeof(int), IPC_CREAT|0X1FF); // create shared memory segment
    if (shm_id < 0)
    {
        perror("shmget fail\n");
    }
    
    int* shmaddr = shmat(shm_id, NULL, 0); // attach processes to shm_id
    *shmaddr = 1;
    printf("value before fork: %d\n", *shmaddr);
    if(fork()==0)
    {
        (*shmaddr) += 1;
        printf("value in the child: %d\n", *shmaddr);
        //for(int i=0; i<1000000; ++ i) (*shmaddr) += 1;
		exit(0);
    }
    else
    {
        wait(NULL);
        printf("value after fork: %d\n", *shmaddr);
		//for(int i=0; i< 1000000; ++i) (*shmaddr) -= 1;
    }
	printf("value after operation: %d\n", *shmaddr);
    return 0;
}
