char getCheckSum(struct packet *packet){
    
    char checkSum=packet->packetType^packet->seqNum;

    for(int i=0;i<sizeof(packet->payload);i++){
        checkSum^=packet->payload[i];
    }

    return checkSum;
}



//for sending packets by threads
void* sendPacket(void *arg){

    struct args *params= (struct args*) arg;

    //sending data
    while(1){
        sendto(params->sockfd,params->packet,sizeof(struct packet), 0, (struct sockaddr*) &(params->addr), sizeof(params->addr));
        printf("Send Packet of Type %d with Sequence No: %d \n",params->packet->packetType,params->packet->seqNum);
        usleep(params->time*1000);     //sleep for 100 ms
        params->packet->seqNum++;
        params->packet->checkSum=getCheckSum(params->packet);
    }

}

//receive and verify checksum
void* recVerifyCheckSum(void *arg){

    struct args *params= (struct args*) arg;

    socklen_t  addr_size=sizeof(params->addr);


    while(1){

        pthread_mutex_lock(&mutex);
        recvfrom(params->sockfd,params->packet,sizeof(struct packet), 0, (struct sockaddr*)&(params->addr),&addr_size);
        if(params->packet->checkSum==getCheckSum(params->packet))
            printf("Verified Packet of Type %d with Sequence No: %d\n",params->packet->packetType,params->packet->seqNum);
        else
            printf("Verification failed for Packet of Type %d with Sequence No: %d\n",params->packet->packetType,params->packet->seqNum);

        pthread_cond_wait(&cond,&mutex);
        pthread_mutex_unlock(&mutex);
    }

}


//process packets of type 1
void* processType1(void *arg){

    struct packet *packet=(struct packet*) arg;


    while(1){
        pthread_mutex_lock(&mutex);
        if(packet->packetType==1){
            printf("Processed Packet of Type 1 with Sequence No: %d and Payload : %s\n",packet->seqNum,packet->payload);
            cp1++;         
            packet->packetType=-1;
            pthread_mutex_unlock(&mutex);
            pthread_cond_signal(&cond);
        }else
            pthread_mutex_unlock(&mutex);
    }
}


//process packets of type 2
void* processType2(void *arg){

    struct packet *packet=(struct packet*) arg;


    while(1){
        pthread_mutex_lock(&mutex);
        if(packet->packetType==2){
            printf("Processed Packet of Type 2 with Sequence No: %d and Payload : %s\n",packet->seqNum,packet->payload);
            cp2++;         
            packet->packetType=-1;
            pthread_mutex_unlock(&mutex);
            pthread_cond_signal(&cond);
        }else
            pthread_mutex_unlock(&mutex);
    }
}


//print number of packets
void* printNumber(void *arg){
    
    while(1){
        printf("Packets of Type 1: %d\n",cp1);
        printf("Packets of Type 2: %d\n",cp2);
        usleep(300000);   //sleep for 300ms
    }

}
