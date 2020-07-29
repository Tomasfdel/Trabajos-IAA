#!/bin/bash

dataFolder="breast_cancer"
dataFile="breast_cancer"
foldAmount=10
foldFolder="${dataFolder}/${foldAmount} Folds"

for ((validationFold = 0; validationFold < foldAmount; validationFold ++))
do
	for((foldNumber = 0; foldNumber < foldAmount; foldNumber ++))
	do
		if(($foldNumber != $validationFold))
		then
			cat "${foldFolder}/${foldNumber}" >> "${validationFold}.data"
		fi
	done

	cp "${foldFolder}/${validationFold}" "${validationFold}.test"
	
	# Naive Bayes
	#~ cp "${dataFolder}/${dataFile}.nb" "${validationFold}.nb"
	#~ trainCount=`wc -l < ${validationFold}.data`
	#~ testCount=`wc -l < ${validationFold}.test`
	#~ sed -i "3s/.*/${trainCount}/" "${validationFold}.nb"
	#~ sed -i "4s/.*/${trainCount}/" "${validationFold}.nb"
	#~ sed -i "5s/.*/${testCount}/" "${validationFold}.nb"
	#~ results=`./nb_n ${validationFold}`
	
	# C4.5
	cp "${dataFolder}/${dataFile}.names" "${validationFold}.names"
	results=`c4.5 -f ${validationFold} -u`
	
	echo "-----  ${validationFold}  -----" >> "ExecutionLogs"
	echo "$results" >> "ExecutionLogs"
	echo "" >> "ExecutionLogs"
	echo "" >> "ExecutionLogs"	
	
	
	rm "${validationFold}.data"
	rm "${validationFold}.test"
	
	# Naive Bayes
	#~ rm "${validationFold}.nb"
	#~ rm "${validationFold}.predic"
	
	# C4.5
	rm "${validationFold}.names"
	rm "${validationFold}.prediction"
	rm "${validationFold}.tree"
	rm "${validationFold}.unpruned"
	
done

# Naive Bayes
#~ mv "ExecutionLogs" "${dataFolder}/Execution Results/Naive Bayes"
# C4.5
mv "ExecutionLogs" "${dataFolder}/Execution Results/C4.5"
