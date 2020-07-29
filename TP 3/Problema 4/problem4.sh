#!/bin/bash

folder="dos_elipses"
set="dos_elipses"

mkdir "Results"

for ((bins = 1; bins <= 200; bins++))
do
    echo "Bins: ${bins}"
    
    sed -i "6s/.*/${bins}/" "${folder}/${set}.nb"
    
	results=`./nb_hist ${folder}/${set}`
	echo "-----  ${bins}  -----" >> "ExecutionLogs"
	echo "$results" >> "ExecutionLogs"
	echo "" >> "ExecutionLogs"
	echo "" >> "ExecutionLogs"

	mv "${folder}"/"${set}.predic" "Results"/"${bins}.predic"

done

mv "ExecutionLogs" "Results/ExecutionLogs"
