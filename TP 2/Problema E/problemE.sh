#!/bin/bash

folder="faces"
set="faces_norm"

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
	mv "${folder}"/"${set}.predic" "Results"/"${i}.predic"

done

mv "ExecutionLogs" "Results/ExecutionLogs"
mv "Results" "${folder}_Results"
rm "1.wts"

