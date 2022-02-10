#! /bin/sh
gcc -std=c99 -Wall -Wextra -pedantic -o shmcr shm_sol.c
gcc -std=c99 -Wall -Wextra -pedantic -o shmuse shmuse.c
