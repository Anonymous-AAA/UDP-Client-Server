#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include "packet.h"
#include "packet.c"



int main(int argc, char **argv){

    //Error showing port number should be specified
    if(argc !=2){
        printf("Usage: %s <port>\n",argv[0]);
        exit(1);
    }


    //creating socket
    char *ip="127.0.0.1";
    int port=atoi(argv[1]);

    int sockfd;
    struct sockaddr_in addr;

    sockfd=socket(AF_INET,SOCK_DGRAM,0);

    if(sockfd<0){
        perror("Socket error\n");
        exit(1);
    }

    memset(&addr,'\0',sizeof(addr));

    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    addr.sin_addr.s_addr=inet_addr(ip);




    //creating two kinds of packets
    struct packet *p1,*p2;


    p1=(struct packet*)malloc(sizeof(struct packet));
    p2=(struct packet*)malloc(sizeof(struct packet));

    //initializing packet of type 1
    p1->packetType=1;
    p1->seqNum=1;
    strcpy(p1->payload,"Packet of type 1");
    p1->checkSum=getCheckSum(p1);


    //initializing packet of type 2
    p2->packetType=2;
    p2->seqNum=1;
    strcpy(p2->payload,"Packet of type 2");
    p2->checkSum=getCheckSum(p2);


    //creating args for threads
    struct args *a1,*a2;

    a1=(struct args*)malloc(sizeof(struct args));
    a2=(struct args*)malloc(sizeof(struct args));

    //initializing arguments for type1;
    a1->packet=p1;
    a1->sockfd=sockfd;
    a1->addr=addr;
    a1->time=100;


    //initializing arguments for type2;
    a2->packet=p2;
    a2->sockfd=sockfd;
    a2->addr=addr;
    a2->time=150;




    //creating threads
    pthread_t t1,t2;

    if(pthread_create(&t1, NULL, &sendPacket, a1)!=0){
        perror("Failed to create thread t1\n");
    }
    

    if(pthread_create(&t2, NULL, &sendPacket, a2)!=0){
        perror("Failed to create thread t2\n");
    }


    if(pthread_join(t1,NULL)!=0){
        perror("Failed to join thread t1\n");
    }


    if(pthread_join(t2,NULL)!=0){
        perror("Failed to join thread t2\n");
    }




    return 0;

}

