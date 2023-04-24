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

    if(argc !=2){
        printf("Usage: %s <port>\n",argv[0]);
        exit(0);
    }

    
    char *ip="127.0.0.1";
    int port=atoi(argv[1]);

    int sockfd;
    struct sockaddr_in server_addr,client_addr;

    int n;


    sockfd=socket(AF_INET,SOCK_DGRAM,0);

    if(sockfd<0){
        perror("Socket error\n");
        exit(1);
    }

    memset(&server_addr,'\0',sizeof(server_addr));

    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(port);
    server_addr.sin_addr.s_addr=inet_addr(ip);

    n=bind(sockfd,(struct sockaddr*) &server_addr,sizeof(server_addr));

    if(n<0){
        perror("Bind error");
        exit(1);
    }

    printf("Server listening on %s:%d\n",ip,port);

    //initializing count variables
    cp1=0;
    cp2=0;

    //allocate memory for storing the received packet 
    struct packet* recv=(struct packet*)malloc(sizeof(struct packet));

    //create args for thread recVerifyCheckSum
    struct args *a=(struct args*)malloc(sizeof(struct args));

    a->packet=recv;
    a->sockfd=sockfd;
    a->addr=client_addr;
    a->time=-1;         //time is irrelevant

    //create threads
    pthread_t t1,t2,t3,t4;


    //initializing mutex
    pthread_mutex_init(&mutex,NULL);

    //intializing condition variable
    pthread_cond_init(&cond, NULL);

    
    //thread to verify checksum
    if(pthread_create(&t1, NULL, &recVerifyCheckSum, a)!=0){
        perror("Failed to create thread t1\n");
    }


    //thread to process packets of type 1
    if(pthread_create(&t2, NULL, &processType1, recv)!=0){
        perror("Failed to create thread t2\n");
    }


    //thread to process packets of type 2
    if(pthread_create(&t3, NULL, &processType2, recv)!=0){
        perror("Failed to create thread t3\n");
    }

    //thread to print number of each packet received
    if(pthread_create(&t4, NULL, &printNumber, NULL)!=0){
        perror("Failed to create thread t4\n");
    }


    if(pthread_join(t1,NULL)!=0){
        perror("Failed to join thread t1\n");
    }

    if(pthread_join(t2,NULL)!=0){
        perror("Failed to join thread t2\n");
    }

    if(pthread_join(t3,NULL)!=0){
        perror("Failed to join thread t3\n");
    }

    if(pthread_join(t4,NULL)!=0){
        perror("Failed to join thread t4\n");
    }

    //freeing mutex
    pthread_mutex_destroy(&mutex);

    //freeing cv
    pthread_cond_destroy(&cond);



    return 0;

}

