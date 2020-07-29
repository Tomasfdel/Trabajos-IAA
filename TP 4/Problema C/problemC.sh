#!/bin/bash

set="diagonal"
executable="../../TP 0/$set"
C=0.78
testN=10000
trainN=250
dList=(2 4 8 16 32)
kList=(1 5 9 13 17 21 35 49 75 99)
iterAmount=20

mkdir "Results"

for d in "${dList[@]}"
do
	echo "Dimension: ${d}"
	
	"$executable" $d $testN $C $RANDOM
	mv "$set.data" "$set.test"
	rm "$set.names"

	mkdir "Results/${d}"
	
	sed -i "1s/.*/${d}/" "${set}.kn"
	
	for k in "${kList[@]}"
	do
		echo "- K: $k"
		sed -i "3s/.*/${k}/" "${set}.kn"	
	
		for ((iteration = 0; iteration < iterAmount; iteration ++))
		do
			echo "-- Iteracion: ${iteration}"
		
			"$executable" $d $trainN $C $RANDOM
			
			results=`./k_nn ${set}`
			echo "-----  ${iteration}  -----" >> "${k}"
			echo "$results" >> "${k}"
			echo "" >> "${k}"
			echo "" >> "${k}"	
		done
	
		mv "${k}" "Results/${d}/${k}"
	
	done
	
done

rm "$set.data"
rm "$set.names"
rm "$set.test"
rm "$set.predic"

