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
dd if=/dev/random bs=1 count=$numbers_count of=numbers status=none
COUNTER=0
         while [  $COUNTER -lt 100 ]; do
             mpirun -q -np $proccessors_count mes numbers $proccessors_count
             let COUNTER=COUNTER+1 
         done



rm -f numbers mes