#!/bin/bash

#preklad cpp zdrojaku
mpic++ -o mes mes.cpp


#vyrobeni souboru s random cisly
dd if=/dev/random bs=1 count=$1 of=numbers

#spusteni
mpirun -np $2 mes

#uklid
rm -f mes numbers
