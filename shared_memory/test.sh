#!/bin/bash

N=$2

sudo taskset -c $N ./shmcr&

if [ $1 != -1 ]
then
	for i in $(seq 0 $1)
	do
		sudo taskset -c `expr $N + $i` ./shmuse&
	done
fi
