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
    key_t key = ftok(".", 'b'); // Create key
    int shm_id = shmget(key, sizeof(int), IPC_CREAT|0X1FF); // Create shared memory segment
    if (shm_id < 0)
    {
        perror("shmget fail\n");
    }
    
    _Atomic int shdint = 0;
	int* shmaddr = shmat(shm_id, NULL, 0); // Attach processes to shm_id
    *shmaddr = shdint;
    printf("value before fork: %d\n", *shmaddr);  

	clock_t initial_time, final_time, start_time, end_time;
	initial_time = clock();

	int n = 10, c = 10000; 
	int i, j, child_status;
	pid_t pid[n];
	for(i=0; i<n; i++)
	{
		if((pid[i] = fork()) == 0){
			start_time = clock();
			for(j=0; j<c; j++) (*shmaddr) += 1;
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
