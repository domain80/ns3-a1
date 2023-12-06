#! /bin/bash

declare sourceNode=0 destinationNode=0
runNum=10
 #30 40 50 60 70 80 90 100
FILE=../../results/statistics.dat
if test -f "$FILE"; then
    echo -n "" > ../../results/statistics.dat
fi

for numNodesVal in 10 20 30 40
do
	sourceNodeVal=1
	destinationNodeVal=$(($numNodesVal-1))
         
	FILE=../../results/statistics.dat
	if test -f "$FILE"; then
    		echo -n "" > ../../results/average.tr
	fi
	for seedVal in 1.0 2.0 3.0 4.0 5.0 6.0 7.0 8.0 9.0 10.0 
  	do
  		 echo $sourceNodeVal "  " $destinationNodeVal

   		 
	 	../../ns3 run "assignment1 --seed=$seedVal --destinationNode=$destinationNodeVal --sourceNode=$sourceNodeVal  --numNodes=$numNodesVal"
                 awk -f assignment1.awk sourceNode=$sourceNodeVal destinationNode=$destinationNodeVal <../../results/output.tr >> ../../results/average.tr
  
  	done
        awk -f statistics.awk numNodes=$numNodesVal <../../results/average.tr >> ../../results/statistics.dat
	#echo > average.tr 
		
done
