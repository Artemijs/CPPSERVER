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
    tcpSetUpSocket();
    udpSetUpSocket(); 
    _allMsgHandles = std::vector<function_ptr>(); 
    //_onUdpMessage =(&iServer::hello);
}
void iServer::hello(int lol){
    std::cout <<"Greetings from "<<(lol)<<"\n";
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
void iServer::tcpSetUpSocket(){
    _tcpSock = socket(AF_INET, SOCK_STREAM, 0);
    if(_tcpSock <0){
        error("ERROR opening socket \n");
    }
   
    struct sockaddr_in serv_addr;
    bzero((char*)&serv_addr, sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(_port);

    if(bind(_tcpSock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
        error("ERROR on binding \n");
    }
    
    listen(_tcpSock, 5);
}
void iServer::udpListen(){
    std::cout<<"UDP Listener Active\n";
    char buff[1024];
    int n;
    struct sockaddr_in from;
    unsigned int fromlen = sizeof(struct sockaddr_in);
    while(!_end){
        n = recvfrom(_udpSock, buff, 1024, 0, (struct sockaddr*)&from, &fromlen);
        if(n < 0) error("ERROR on receive\n");
        //_onTcpMessage(0);
        
        printf( "Received a datagram from: %s\n", inet_ntoa(from.sin_addr));
        //_onUdpMessage(buff, from);
        //(*this.*_onUdpMessage)(10);
        
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
void iServer::tcpListen(){
    std::cout<<"Tcp Listener Active\n";
    int newsockfd, pid;
    while   (1){
        newsockfd = tcpAcceptConns(); 
        pid = fork();
        if(pid < 0){
            error("ERROR on fork \n");
        }
        if(pid == 0){
            close(_tcpSock);//?
            tcpHandleConns(newsockfd);
            exit(0);
        }
        else
            close(newsockfd);
    }
    close(_tcpSock);
    

}
int iServer::getMsgId(char* buff){
    char nr[2];
    for(int i =0; i < 2; i++){
        nr[i] = buff[i];
    }
    return atoi(nr);
}

void iServer::tcpHandleConns(int clientSock){
    while(1){
        int n;
        char buffer[256]; 
        
        bzero(buffer, 256);
        n = read(clientSock, buffer, 255);
        if(n < 0) error("ERROR on reading from socket\n");
        if(n == 0) return;
        //i had it passed an arg to _onTcpMessage but that gave me segmentation errors
        //nowi store this in a global variable and use it inside the function anyway
        _tcpMsg = buffer;

        (*_child.*_onTcpMessage)(10, "GRRRRRRRRRRRR");
        //(*this.*_onUdpMessage)(10);
        //_onTcpMessage( clientSock);
        //struct sockaddr_in lol;
        //_onUdpMessage(buffer, lol);
        // 00 00 data the first 2 are the enum id
        //int id = getMsgId(buffer);
        //printf("client said %s\n",buffer);
        //_allMsgHandles[id](buffer, clientSock);
        
        n = write(clientSock, "Got your message", 16);  
        if( n < 0 ) error("ERROR write to socket\n");
    }

}

int iServer::tcpAcceptConns(){
    unsigned int clilen;
    struct sockaddr_in cli_addr;
    clilen = sizeof(cli_addr);
    int newConn = accept(_tcpSock, (struct sockaddr*)& cli_addr, &clilen);   
    
    if(newConn < 0){
        error("ERROR on accept\n");
    }
    std::cout<<"connected "<<cli_addr.sin_addr.s_addr<<"\n";   
    return newConn;

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

