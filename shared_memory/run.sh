#! /bin/sh
read -p "How many process(N): " N

c=0 # For case A
taskset -c $c ./shmcr &

i=1
while [ $i -lt $N ]
do
	taskset -c $c ./shmuse &
	i=$((i+1))
done

echo "\n"
