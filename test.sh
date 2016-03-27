#!/bin/bash

#author: Martin Maga (xmagam00)
# PRL1: Minimum Extraction Sort

if [ $# != 2 ]; then
	echo "Number of arguments can be only 2"
	exit 1
fi

numbers_count=$1
proccessors_count=$2

#compile program
mpicc --prefix /usr/local/share/OpenMPI -o mes mes.c
#create random numbers
dd if=/dev/random bs=1 count=$numbers_count of=numbers status=none
mpirun --prefix /usr/local/share/OpenMPI -q -np $proccessors_count mes $proccessors_count

rm -f numbers mes