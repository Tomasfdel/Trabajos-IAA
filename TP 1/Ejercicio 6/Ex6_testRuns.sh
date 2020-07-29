#!/bin/bash

set="horizontal"
executable="../../TP 0/$set"
d=5
testN=10000
trainN=250
cList=("0.5" "1.0" "1.5" "2.0" "2.5")
iterAmount=20



for C in "${cList[@]}"
do
	"$executable" $d $testN $C $RANDOM
	mv "$set.data" "$set.test"
	rm "$set.names"
	
	prePrunAcum="0"
	postPrunAcum="0"
	for ((iteration = 0; iteration < iterAmount; iteration ++))
	do
		"$executable" $d $trainN $C $RANDOM
		results=`c4.5 -f $set -u | grep "<<"`

		errorPrePrun=`echo "$results" | sed -n "2p" | cut -d"(" -f2 | cut -d"%" -f1 | awk '{$1=$1};1'`
		errorPostPrun=`echo "$results" | sed -n "2p" | cut -d"(" -f3 | cut -d"%" -f1 | awk '{$1=$1};1'`

		prePrunAcum=`echo "$prePrunAcum + $errorPrePrun" | bc`
		postPrunAcum=`echo "$postPrunAcum + $errorPostPrun" | bc`
	done
	prePrunAcum=`echo "scale=3; $prePrunAcum / $iterAmount" | bc`
	postPrunAcum=`echo "scale=3; $postPrunAcum / $iterAmount" | bc`
	echo "C: $C, PrePrun: $prePrunAcum - PostPrun: $postPrunAcum"
	echo "$C $prePrunAcum" >> "${set}_Before-Prunning"
	echo "$C $postPrunAcum" >> "${set}_After-Prunning"
done

rm "$set.data"
rm "$set.names"
rm "$set.test"
rm "$set.prediction"
rm "$set.tree"
rm "$set.unpruned"


