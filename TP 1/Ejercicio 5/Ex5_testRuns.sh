#!/bin/bash

set="horizontal"
executable="../../TP 0/$set"
C=0.78
d=2
testN=10000
nList=(125 250 500 1000 2000 4000)
iterAmount=20

"$executable" $d $testN $C $RANDOM
mv "$set.data" "$set.test"
rm "$set.names"

for n in "${nList[@]}"
do
	trainingAcum="0"
	testAcum="0"
	sizeAcum="0"	
	for ((iteration = 0; iteration < iterAmount; iteration ++))
	do
		"$executable" $d $n $C $RANDOM
		results=`c4.5 -f $set -u | grep "<<"`
		
		errors=`echo "$results" | cut -d"(" -f3 | cut -d"%" -f1`
		trainingError=`echo "${errors}" | sed -n "1p" | awk '{$1=$1};1'`
		testError=`echo "${errors}" | sed -n "2p" | awk '{$1=$1};1'`
		
		sizes=`echo "$results" | cut -d")" -f2 | cut -f1`
		size=`echo "${sizes}" | sed -n "1p" | awk '{$1=$1};1' | cut -d" " -f1`
		
		trainingAcum=`echo "$trainingAcum + $trainingError" | bc`
		testAcum=`echo "$testAcum + $testError" | bc`
		sizeAcum=`echo "$sizeAcum + $size" | bc`
	done
	trainingAcum=`echo "scale=3; $trainingAcum / $iterAmount" | bc`
	testAcum=`echo "scale=3; $testAcum / $iterAmount" | bc`
	sizeAcum=`echo "scale=3; $sizeAcum / $iterAmount" | bc`
	echo "$n, Training: $trainingAcum - Test: $testAcum - Size: $sizeAcum"
	echo "$n $trainingAcum" >> "${set}_TrainingErrors"
	echo "$n $testAcum" >> "${set}_TestErrors"
	echo "$n $sizeAcum" >> "${set}_TreeSizes"
done

rm "$set.data"
rm "$set.names"
rm "$set.test"
rm "$set.prediction"
rm "$set.tree"
rm "$set.unpruned"


