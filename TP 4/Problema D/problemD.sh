#!/bin/bash

set="horizontal"
executable="../../TP 0/$set"
C=0.78
testN=10000
trainN=250
dList=(2 4 8 16 32)
distList=("0.0001", "0.0005", "0.001", "0.005", "0.01", "0.05", "0.1", "0.5", "1", "5", "10", "20", "40", "50", "100", "500", "1000", "10000")
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
	
	for dist in "${distList[@]}"
	do
		echo "- Distancia: $dist"
		sed -i "3s/.*/${dist}/" "${set}.kn"	
	
		for ((iteration = 0; iteration < iterAmount; iteration ++))
		do
			echo "-- Iteracion: ${iteration}"
		
			"$executable" $d $trainN $C $RANDOM
			
			results=`./d_nn ${set}`
			echo "-----  ${iteration}  -----" >> "${dist}"
			echo "$results" >> "${dist}"
			echo "" >> "${dist}"
			echo "" >> "${dist}"	
		done
	
		mv "${dist}" "Results/${d}/${dist}"
	
	done
	
done

rm "$set.data"
rm "$set.names"
rm "$set.test"
rm "$set.predic"

