#!/bin/bash

folder="espirales_con_ruido"
set="c_0"
kList=(1 5 9 13 17 21 35 49 75 99)
iterAmount=20

mkdir "Results"

for k in "${kList[@]}"
do
	echo "K: $k"
	sed -i "3s/.*/${k}/" "${folder}/${set}.kn"
	mkdir "Results/${k}"
	
	for ((iteration = 0; iteration < iterAmount; iteration ++))
	do
		echo "-- Iteracion: ${iteration}"
		
		results=`./k_nn ${folder}/${set}`
		echo "-----  ${iteration}  -----" >> "${k}"
		echo "$results" >> "${k}"
		echo "" >> "${k}"
		echo "" >> "${k}"	
		mv "${folder}/${set}.predic" "Results/${k}/${iteration}.predic"
	
	done
	
	mv "${k}" "Results/${k}/ExecutionLogs"

done
