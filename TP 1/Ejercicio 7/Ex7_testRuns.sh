#!/bin/bash

set="horizontal"
executable="../../TP 0/$set"
C=0.78
testN=10000
trainN=250
dList=(2 4 8 16 32)
iterAmount=20

for d in "${dList[@]}"
do
	"$executable" $d $testN $C $RANDOM
	mv "$set.data" "$set.test"
	rm "$set.names"

	trainingAcum="0"
	testAcum="0"	
	for ((iteration = 0; iteration < iterAmount; iteration ++))
	do
		"$executable" $d $trainN $C $RANDOM
		results=`c4.5 -f $set -u | grep "<<"`
		
		errors=`echo "$results" | cut -d"(" -f3 | cut -d"%" -f1`
		trainingError=`echo "${errors}" | sed -n "1p" | awk '{$1=$1};1'`
		testError=`echo "${errors}" | sed -n "2p" | awk '{$1=$1};1'`
		
		trainingAcum=`echo "$trainingAcum + $trainingError" | bc`
		testAcum=`echo "$testAcum + $testError" | bc`
	done
	trainingAcum=`echo "scale=3; $trainingAcum / $iterAmount" | bc`
	testAcum=`echo "scale=3; $testAcum / $iterAmount" | bc`
	echo "$d, Training: $trainingAcum - Test: $testAcum"
	echo "$d $trainingAcum" >> "${set}_TrainingErrors"
	echo "$d $testAcum" >> "${set}_TestErrors"
done

rm "$set.data"
rm "$set.names"
rm "$set.test"
rm "$set.prediction"
rm "$set.tree"
rm "$set.unpruned"


