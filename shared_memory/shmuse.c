#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main (void) {
	int shm_id;
	/*volatile*/ int key = 185749;
	/*volatile*/ int *mem;
	void *mem_seg = NULL;
	int j;
	clock_t times;

	if ((shm_id = shmget(key, sizeof(int), 0666)) == -1) {
		printf ("shm create failed\n");

		exit (-1);
	}

	if ((mem_seg = shmat (shm_id, NULL,0)) == (void*) -1) {
		printf ("shmat failed\n");

		exit (-1);
	}

	mem = (int *)mem_seg;
	// printf("original mem : %d\n", (*mem));
	// *mem = 0;

	times = clock ();
	
	if(fork()==0) execl("/usr/bin/free", "free", "-w", 0);
    wait(NULL);
	for (j = 0; j < 1000000; j++) 	if(*mem == 100);	//__atomic_fetch_add (mem, 1, __ATOMIC_SEQ_CST);
	
	if(fork()==0) execl("/usr/bin/free", "free", "-w", 0);
    wait(NULL);
	
	times = clock () - times;

	printf ("\nPID %d: %lf\n\n", getpid(), (double)(times)/CLOCKS_PER_SEC);
	// printf ("Final mem : %d\n", (*mem));
	
	return 0;
}
