#!/bin/bash

folder="dos_elipses"
set="dos_elipses"

mkdir "Results"

for ((i = 0; i < 10; i++))
do
    echo "Iteracion: ${i}"
	results=`./bp ${folder}/${set}`
	echo "-----  ${i}  -----" >> "ExecutionLogs"
	echo "$results" >> "ExecutionLogs"
	echo "" >> "ExecutionLogs"
	echo "" >> "ExecutionLogs"

	mv "${folder}"/"${set}.mse" "Results"/"${i}.mse"

done

mv "ExecutionLogs" "Results/ExecutionLogs"
rm "1.wts"
rm "${folder}"/"${set}.predic"
