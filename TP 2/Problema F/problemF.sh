#!/bin/bash

folder="ssp"
set="ssp"
iterAmount=10
batchList=("1" "5" "20" "171")

mkdir "Results"

for batch in "${batchList[@]}"
do
	echo "Batch: ${batch}"
	
	mkdir "Results"/"${batch}"
	
	sed -i "10s/.*/${batch}/" "${folder}/${set}.net"
	
	for ((i = 0; i < 10; i++))
	do
		echo "Iteracion: ${i}"
		results=`./bp ${folder}/${set}`
		echo "-----  ${i}  -----" >> "ExecutionLogs"
		echo "$results" >> "ExecutionLogs"
		echo "" >> "ExecutionLogs"
		echo "" >> "ExecutionLogs"

		mv "${folder}"/"${set}.mse" "Results"/"${batch}"/"${i}.mse"

	done
	
	mv "ExecutionLogs" "Results"/"${batch}"/"ExecutionLogs"
	
done


rm "1.wts"
rm "${folder}"/"${set}.predic"
