#!/bin/awk -f
BEGIN {

	recvdPacket_size = 0;
	NumOfsentPackets=0;
	recievedPackets=0;
	startTime = 1e6;
	stopTime = 0;
	sequenceNo = -1;
	throughput = 0;
	etoeDelay = 0;
	numOfRecvdPackets = 0;
	#sourceNode = ARGV[1];
	#destinationNode = ARGV[2];
}
{
	event = $1;
	time = $2;	split($3, n, "/");
	split($4, r, "/");
	node_id = n[3];
	recieved_node_id = r[3];
	flow_type = (event=="t" || event=="+") ? $53 : $53;

	if( (event=="t" || event=="+") && flow_type=="Payload" ) {
		packet_id = $31;
		packet_size = substr($54,7,4);
		NumOfsentPackets+=1;
		if(node_id == sourceNode && packet_id == 0) {
			if (time < startTime) {
				startTime = time;
			}
		
	
	    }

		if(sequenceNo < packet_id) {
			sequenceNo = packet_id;
			seqSendTime[sequenceNo] = time;
		}
	}

	if(event=="r" && flow_type=="Payload") {
		packet_id = $31;
		packet_size = substr($54,7,4);
		
		if(recieved_node_id == destinationNode) {
			if (time > stopTime) {
				stopTime = time;
			}
			recvdPacket_size += packet_size;
			recvTime[packet_id] = time;
			numOfRecvdPackets += 1;
	    }
	}
}
END {

	throughput = (recvdPacket_size / (stopTime - startTime) * (8 / 1000));
	for(i=0; i <= sequenceNo; i++){
		if(recvTime[i] > 0){
			packetDelay = recvTime[i] - seqSendTime[i];
			if(packetDelay > 0) {
				etoeDelay += packetDelay;
			}
		}
	}
	
        printf("PacketSent: %d", NumOfsentPackets);
	printf("  PacketRecvd: %d", numOfRecvdPackets);
	printf("  StartTime: %f", startTime);
	printf("  StopTime: %f",  stopTime);
	printf("  RecievedPacketSize: %d", recvdPacket_size);
	printf("  Throughput: %f", throughput);
	printf("  End-to-EndDelay: %f",etoeDelay/numOfRecvdPackets);
        printf("  Destination Node  %d\n", destinationNode);

}
