#!/bin/bash

folder="ssp"
set="ssp"
iterAmount=10
expList=("0" "-1" "-2" "-3" "-4" "-5" "-6" "-7" "-8")

mkdir "Results"

for exp in "${expList[@]}"
do
	echo "Exponente: ${exp}"
	
	mkdir "Results"/"${exp}"
	
	sed -i "10s/.*/1e${exp}/" "${folder}/${set}.net"
	
	for ((i = 0; i < 10; i++))
	do
		echo "Iteracion: ${i}"
		results=`./bp ${folder}/${set}`
		echo "-----  ${i}  -----" >> "ExecutionLogs"
		echo "$results" >> "ExecutionLogs"
		echo "" >> "ExecutionLogs"
		echo "" >> "ExecutionLogs"

		mv "${folder}"/"${set}.mse" "Results"/"${exp}"/"${i}.mse"

	done
	
	mv "ExecutionLogs" "Results"/"${exp}"/"ExecutionLogs"
	
done


rm "1.wts"
rm "${folder}"/"${set}.predic"
