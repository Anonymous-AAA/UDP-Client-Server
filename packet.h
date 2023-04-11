struct packet{

    int packetType;
    int seqNum;
    char payload[1024];
    char checkSum;

};

struct args{

    struct packet *packet;
    int sockfd;
    struct sockaddr_in addr;
    int time;  //in ms
};


pthread_mutex_t mutex;
pthread_cond_t cond;
int cp1,cp2;
