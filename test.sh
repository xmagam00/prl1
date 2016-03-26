#!/bin/bash

if [ $# != 2 ]; then
	echo "Number of arguments can be only 2"
	exit 1
fi
numbers_count=$1
proccessors_count=$2

#compile program
mpicc -o mes mes.c
#create random numbers
dd if=/dev/random bs=1 count=$numbers_count of=numbers 
mpirun -np $proccessors_count mes numbers $proccessors_count | grep -v *"records"*
rm -f numbers mes