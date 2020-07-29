#!/bin/bash

set="diagonal"
executable="../../TP 0/$set"
d=5
testN=10000
cList=("0.5" "1.0" "1.5" "2.0" "2.5")


for C in "${cList[@]}"
do
	"$executable" $d $testN $C $RANDOM
	bayesError=`python3 ${set}_Bayes.py`
	echo "$C $bayesError" >> "${set}_Bayes"
done

rm "$set.data"
rm "$set.names"

