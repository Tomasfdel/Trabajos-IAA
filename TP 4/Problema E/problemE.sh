#!/bin/bash

folder="ssp"
set="ssp"
kList=(1 2 3 4 5 6 7 8 9 10 12 14 16 18 20 23 26 29 32 35 39 43 47 50)
iterAmount=20

mkdir "Results"

for k in "${kList[@]}"
do
	echo "K: $k"
	sed -i "2s/.*/${k}/" "${folder}/${set}.kn"
	mkdir "Results/${k}"
	
	for ((iteration = 0; iteration < iterAmount; iteration ++))
	do
		echo "-- Iteracion: ${iteration}"
	
		sed -i "6s/.*/$RANDOM/" "${folder}/${set}.kn"
		
		results=`./k_nn_r ${folder}/${set}`
		echo "-----  ${iteration}  -----" >> "${k}"
		echo "$results" >> "${k}"
		echo "" >> "${k}"
		echo "" >> "${k}"	
		mv "${folder}/${set}.predic" "Results/${k}/${iteration}.predic"
	
	done
	
	mv "${k}" "Results/${k}/ExecutionLogs"

done
