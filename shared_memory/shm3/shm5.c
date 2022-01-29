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
    
    _Atomic int shdint = 1;
	int* shmaddr = shmat(shm_id, NULL, 0); // Attach processes to shm_id
    *shmaddr = shdint;
    printf("value before fork: %d\n", *shmaddr);  

	clock_t initial_time, final_time, start_time, end_time;
	initial_time = clock();

	int lock = 0; // Initially lock=FALSE

	int n = 10, t = 1000000; 
	int i, j, child_status;
	pid_t pid[n];
	int shm_id[n];
	int* shmaddr[n];
	for(i=0; i<n; i++)
	{
		shm_id[i] = shmget(k, sizeof(int), IPC_CREAT|0X1FF);
		if(shm_id[i] < 0){
			perror("shmget fail\n");
		}
		smaddr[i] = shmat(shm_id, NULL, 0);
	}
	int *shmaddr[n];
	for(i=0; i<n; i++)
	{
		if((pid[i] = fork()) == 0){
			start_time = clock();
			for(j=0; j<t; j++) (*shmaddr) += 1;
			end_time = clock();
			printf("value in the child %d: %d (runtime: %lfms)\n", getpid(), *shmaddr, (double)(end_time - start_time));	
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
	final_time = clock();
	printf("Total runtime: %lfms\n", (double)(final_time - initial_time));
	
	return 0;
}
