#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(void)
{
	int n = 10, c = 1000000;
	int i, j, child_status;
	clock_t initial_time, final_time, start_time, end_time;
	initial_time = clock();

	pid_t pid[n];
	for(i=0; i<n; i++)
	{
		if((pid[i] = fork()) == 0){
   			key_t key = 90000+i; // create key
    		int shm_id = shmget(key, sizeof(int), IPC_CREAT|0X1FF); // create shared memory segment
    		if (shm_id < 0)
    		{
     	   		perror("shmget fail\n");
	    	}
   
			_Atomic int shdint = 1; 
  	  		int* shmaddr = shmat(shm_id, NULL, 0); // attach processes to shm_id
   			*shmaddr = shdint;
			
			start_time = clock();
        	for(j=0; j<c; j++) (*shmaddr) += 1;
			end_time = clock();
			printf("value in the child %d: %d(runtime: %lfms)\n", getpid(), *shmaddr, (double)(end_time - start_time));

			exit(100+i);
		}
	}
	
	for(i=0; i<n; i++)
	{
		pid_t wpid = wait(&child_status);
		if(WIFEXITED(child_status))
			printf("Child %d terminated with exit status %d\n", wpid, WEXITSTATUS(child_status));
		else
			printf("Child %d terminated abnormally\n", wpid);
	}
	final_time = clock();
	printf("Total runtime: %lfms\n", (double)(final_time - initial_time));
	
    return 0;
}
