#!/bin/bash

set="diagonal"
executable="../../TP 0/$set"
C=0.78
testN=10000
trainN=250
dList=(2 4 8 16 32)
iterAmount=20

mkdir "Results"

for d in "${dList[@]}"
do
	echo "Dimension: ${d}"
	
	sed -i "1s/.*/${d}/" "${set}.nb"
	
	"$executable" $d $testN $C $RANDOM
	mv "$set.data" "$set.test"
	rm "$set.names"
	
	for ((iteration = 0; iteration < iterAmount; iteration ++))
	do
		echo "- Iteracion: ${iteration}"
	
		"$executable" $d $trainN $C $RANDOM
		
		results=`./nb_n ${set}`
		echo "-----  ${iteration}  -----" >> "${d}"
		echo "$results" >> "${d}"
		echo "" >> "${d}"
		echo "" >> "${d}"	
	
	done
	
	mv "${d}" "Results"/"${d}"
done

rm "$set.data"
rm "$set.names"
rm "$set.test"
rm "$set.predic"

