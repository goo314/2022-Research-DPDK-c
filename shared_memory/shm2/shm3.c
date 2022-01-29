#include <sys/ipc.h>
#include <sys/shm.h>
#include<stdio.h>
#include<stdatomic.h>
#include<pthread.h>
#include <time.h>
#include <unistd.h>

_Atomic int* shmaddr;
void *adding(void *input)
{
    for(int i=0; i<10000; i++)
    {
        *shmaddr += 1;
    }
	printf("value in the thread %lu: %d\n", pthread_self(), *shmaddr);
    pthread_exit(NULL);
}
int main()
{
	key_t k = ftok(".", 'b'); // Create key
    int shm_id = shmget(k, sizeof(int), IPC_CREAT|0X1FF); // Create shared memory segment
    if (shm_id < 0)
    {
        perror("shmget fail\n");
    }
    
	shmaddr = shmat(shm_id, NULL, 0); // Attach processes to shm_id
    *shmaddr = 0;

    pthread_t tid[10];
    clock_t start_time, end_time;
	start_time = clock();

	for(int i=0; i<10; i++)
        pthread_create(&tid[i],NULL,adding,NULL);
    for(int i=0; i<10; i++)
        pthread_join(tid[i],NULL);
 
	printf("the value of *shmaddr is %d\n", *shmaddr);
    
	end_time = clock();
	printf("Runtime: %lfms\n", (double)(end_time - start_time));

	return 0;
}
