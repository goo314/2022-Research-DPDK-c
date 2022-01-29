#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdio.h>
#include <time.h>


int main(void)
{
    key_t k = ftok(".", 'b'); // Create key
    int shm_id = shmget(k, sizeof(int), IPC_CREAT|0X1FF); // Create shared memory segment
    if (shm_id < 0)
    {
        perror("shmget fail\n");
    }
    
	_Atomic int* shmaddr = shmat(shm_id, NULL, 0); // Attach processes to shm_id
    *shmaddr = 0;
    printf("value before fork: %d\n", *shmaddr);  

	clock_t start_time, end_time;
	start_time = clock();

	int lock = 0; // Initially lock=FALSE

	int n = 5, i, child_status;
	pid_t pid[n];
	for(i=0; i<n; i++)
	{
		if((pid[i] = fork()) == 0){
			(*shmaddr) += 1;
			printf("value in the child %d: %d\n", getpid(), *shmaddr);
			exit(100+i);
		}
	}
	
	for(i=0; i<n; i++)
	{
		pid_t wpid = wait(&child_status);
		if(WIFEXITED(child_status))
			printf("Child %d terminated with exit status %d\n", wpid, WEXITSTATUS(child_status));
		else
			printf("Child %d terminate abnormally\n", wpid);
	}

	end_time = clock();
	printf("Runtime: %lfms\n", (double)(end_time - start_time));

	return 0;
}
