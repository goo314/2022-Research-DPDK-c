#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main (int argc, char* args[]) {
	int NP = atoi(args[1]);
	int shm_id;
	int key = 185749;
	int *mem;
	void *mem_seg = NULL;
	int i, j, n;
	int pids[NP];
	clock_t times;

	int procs = NP;
	if ((shm_id = shmget(key, sizeof(int), IPC_CREAT | 0666)) == -1) {
		printf ("shm create failed\n");

		exit (-1);
	}

	if ((mem_seg = shmat (shm_id, NULL,0)) == (void*) -1) {
		printf ("shmat failed\n");

		exit (-1);
	}

	mem = (int *)mem_seg;
	*mem = 0;

	for (i = 0; i < NP; i++) {
		if ((pids[i] = fork ()) == 0) {
			times = clock ();
			// for (j = 0; j < 1000000; j++) 		(*mem) += 1;
			for (j = 0; j < 1000000; j++) 		__atomic_fetch_add (mem, 1, __ATOMIC_SEQ_CST);
			
			times = clock () - times;

			printf ("elapse time : %lf\n", (double)(times)/CLOCKS_PER_SEC);
			exit (i);
		}
	}

	int ret;
	int ret_pid;
	while (procs > 0) {
		ret_pid = wait (&ret);
		if (ret_pid > 0) {
			printf ("pid %d return with ret %d %d\n", ret_pid, ret, procs);
			--procs;
		}
	}

	printf ("Final mem : %d\n", (*mem));
	
	return 0;
}
