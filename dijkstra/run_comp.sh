#!/bin/bash

N=(2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576)
M=(2 4 8 16 32 64 128 256 512 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576)

n=1048576
m=1048576

mkdir "rcomp"

for n in ${N[*]}
do
	
	./gen -n $n -m $m > "temp.gr"
	outf="./rcomp/RC-"$n"."$m".dat"
	./exp -t s -n 50 -h 2 < "temp.gr" > $outf 	
	
	echo "Done with "$n
	echo "Results in file "$outf
		
done

rm "temp.gr"
