#include "server.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <netdb.h>
#include "servcalls.h"
iServer::iServer(){}

iServer::iServer(int port, Server* childPtr) : _port(port), _end(false), _child(childPtr){
    udpSetUpSocket(); 
}



void iServer::udpSetUpSocket(){
    struct sockaddr_in server;

    _udpSock = socket(AF_INET, SOCK_DGRAM, 0);
    if(_udpSock <0) error("ERROR creating sock\n");
    
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(_port+1);
    
    if(bind(_udpSock, (struct sockaddr*)&server, sizeof(server)) < 0)
        error("ERROR on bind\n");

}

void iServer::udpListen(std::vector<function_ptr>* ptr ){
    std::cout<<"UDP Listener Active\n";
    char buff[1024];
    memset(buff, 0x00, 1024);
    int n;
    struct sockaddr_in from;
    unsigned int fromlen = sizeof(struct sockaddr_in);
    while(!_end){
        
        n = recvfrom(_udpSock, buff, 1024, 0, (struct sockaddr*)&from, &fromlen);
        if(n < 0) error("ERROR on receive\n");
        //printf( "Received a datagram from: %s\n", inet_ntoa(from.sin_addr));
        (*_child.*_onUdpMessage)(buff, from, ptr);
        
        //write(1, buff, n);
        //std::cout<<"\n";
       /* if(buff[0] == '-' && buff[1] == 'n' && buff[2] == '-'){
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

        }*/
    }
    std::cout<<"ENDING THE UDP LOOP\n";
}
int iServer::getMsgId(char* buff){
    char nr[2];
    for(int i =0; i < 2; i++){
        nr[i] = buff[i];
    }
    return atoi(nr);
}



void iServer::error(const char* msg){
    perror(msg);
    exit(0);
}

void iServer::end(){
    std::cout<<"End called\n";
    _end = true;

    struct sockaddr_in serv_addr;
    struct hostent *hp;
    hp = gethostbyname("localhost");
    if(hp == 0) error("UNKNOVVN HOST");
    serv_addr.sin_family= AF_INET;
    serv_addr.sin_port = htons(_port+1);    
    bcopy((char*)hp->h_addr, (char *)&serv_addr.sin_addr, hp->h_length);
    
    sendto(_udpSock, "E", 1, 0, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr_in));    
}

void iServer::cleanUp(){
}

int iServer::getMsgLen(char* buff, int offset){
    char nr[2];
    for(int i =0; i < 2; i++){
        nr[i] = buff[offset+i];
    }
    return atoi(nr);
}

void iServer::getLenStr(char* str, int len){
    std::string strlen = std::to_string(len);
    if(strlen.length() < 2){
        str[0] = '0';
        str[1] = strlen[0];
    }
    else{
        str[0] = strlen[0];
        str[1] = strlen[1];
    }
     
}
