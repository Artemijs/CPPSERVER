#include <iostream>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
void error(const char* msg){
    perror(msg);
    exit(0);
}
int getMsgLen(char* buff, int offset){
    int len = 0;
    char nr[2];
    for(int i =0; i < 2; i++){
        nr[i] = buff[offset+i];
    }
    return atoi(nr);
}
void removeConnection(sockaddr_in* all_cons, int len, sockaddr_in* con){
    bool removed = false;
    for(int i =0; i < len; i++){
        if(all_cons[i].sin_addr.s_addr == con->sin_addr.s_addr){
            //remove it and move everything ahead of this item back by 1
            //all_cons[i] = NULL;
            if(i == len -1) break;
            removed = true;
        } 
        if(removed){
            all_cons[i-1] = all_cons[i];
        }
    }
}
//  n = sendto(sock, buff, msgLen+5, 0, (struct sockaddr*)&all_conns[i], fromlen);

void sendToAll(sockaddr_in* all_conns, int connCount, int sock, char* buff, int msgLen){
    int fromlen = sizeof(struct sockaddr_in);
    for(int i =0; i <connCount; ++i){
        int n = sendto(sock, buff, msgLen, 0, (struct sockaddr*)&all_conns[i], fromlen);
        if(n < 0)error("sendto");

    }
}
void sendToAllOthers(sockaddr_in* all_conns,  int connCount, int sock, char* buff, int msgLen, sockaddr_in* notSend){
    int fromlen = sizeof(struct sockaddr_in);
    for(int i =0; i <connCount; ++i){
        
        if(all_conns[i].sin_addr.s_addr != notSend->sin_addr.s_addr){
            int n = sendto(sock, buff, msgLen, 0, (struct sockaddr*)&all_conns[i], fromlen);
            if(n < 0)error("sendto");
        }

    }
}

int main( int argc, char* argv[]){
    printf("online\n");
    int sock, length, n;
    unsigned int fromlen;
    struct sockaddr_in server, from;
    char buff[1024];
    int connCount =0;
    struct sockaddr_in all_conns[10];    
    
    if(argc <2){
        fprintf(stderr, "ERROR, no port provided\n");
        exit(0);
    }    
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock <0) error("ERROR creating sock\n");
    
    length = sizeof(server);
    bzero(&server, length);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(atoi(argv[1]));
    
    if(bind(sock, (struct sockaddr*)&server, length) < 0)
        error("ERROR on bind\n");

    fromlen = sizeof(struct sockaddr_in);
    
    while(1){
        n = recvfrom(sock, buff, 1024, 0, (struct sockaddr*)&from, &fromlen);
        if(n < 0) error("ERROR on receive\n");
        printf( "Received a datagram from: %s\n", inet_ntoa(from.sin_addr));
        
        write(1, buff, n);
        std::cout<<"\n";
        if(buff[0] == '-' && buff[1] == 'n' && buff[2] == '-'){
            //create new connection 
            all_conns[connCount] = from;
            connCount++;
            std::cout<<"Connections : "<<connCount<<"\n";
            int msgLen = getMsgLen(buff, 3);
            sendToAll(all_conns, connCount, sock, buff, msgLen+5);
        } 
        if(buff[0] == '-' && buff[1] == 'e' && buff[2] == '-'){
            int msgLen = getMsgLen(buff, 3);
            sendToAllOthers(all_conns, connCount, sock, buff, msgLen+5, &from);
            removeConnection( all_conns, connCount, &from);
            connCount--;
        }
        if(buff[0] == '-' && buff[1] == 'm' && buff[2] == '-'){
            int msgLen = getMsgLen(buff, 3);
            sendToAllOthers(all_conns, connCount, sock, buff, msgLen+5, &from);

        }
    }
    return 0;
}
