#!/bin/awk -f
BEGIN {

	averagePacketSent=0;
	averagePacketRec=0;
	averageThroughput=0;
	averageEtEDelay=0;
}
{

	averagePacketSent+=$2;
	averagePacketRec+=$4;
	averageThroughput+=$12;
	averageEtEDelay+=$14;
	
}
END {

	averagePacketSent/=NR;
	averagePacketRec/=NR;
	averageThroughput/=NR;
	averageEtEDelay/=NR;
	
	
	printf("%d", numNodes);
        printf("  %f", averagePacketSent);
	printf("  %f", averagePacketRec);
	printf("  %f", averageThroughput);
	printf("  %f\n",  averageEtEDelay);
	

}
