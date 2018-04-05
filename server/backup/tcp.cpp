#include <iostream>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <arpa/inet.h>
void error(const char *msg){
    perror(msg);
    exit(1);
}
int setupSocket(int portno){
    int socketId = socket(AF_INET, SOCK_STREAM, 0);
    if(socketId <0){
        error("ERROR opening socket \n");
    }
   
    struct sockaddr_in serv_addr;
    bzero((char*)&serv_addr, sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if(bind(socketId, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
        error("ERROR on binding \n");
    }
    
    listen(socketId, 5);
    return socketId;
}
int acceptConnections(int serv_sock){
    unsigned int clilen;
    struct sockaddr_in cli_addr;
    clilen = sizeof(cli_addr);
    int newConn = accept(serv_sock, (struct sockaddr*)& cli_addr, &clilen);   
    if(newConn < 0){
        error("ERROR on accept\n");
    }
    
    std::cout<<"connected "<<cli_addr.sin_addr.s_addr<<"\n";   
    return newConn;
}
void dostuff(int clientSock){
    while(1){
        int n;
        char buffer[256]; 
        
        bzero(buffer, 256);
        n = read(clientSock, buffer, 255);
        if(n < 0) error("ERROR on reading from socket\n");
        if(n == 0) return;
        printf("client said %s\n",buffer);
        
        n = write(clientSock, "Got your message", 16);  
        if( n < 0 ) error("ERROR write to socket\n");
    }
}



void listen( int sockfd){
    std::cout<<"Tcp Listener Active\n";
    int newsockfd, pid;
    while(1){
        newsockfd = acceptConnections(sockfd); 
        pid = fork();
        if(pid < 0){
            error("ERROR on fork \n");
        }
        if(pid == 0){
            close(sockfd);//?
            dostuff(newsockfd);
            exit(0);
        }
        else
            close(newsockfd);
    }
    close(sockfd);
    
    std::cout<<"Ending CHILD process\n";
}
bool _end;
void listenUdp(){
    while(!_end){
        
    }
}
void sighandle(int signo){
    std::cout<<"Got a signal : "<<signo<<std::endl;
    exit(signo);
}
void ctrlhandle(int signo){
    std::cout<<"Got a signal : "<<signo<<std::endl;
    _end = true;
}
int main(int argc, char* argv[]){
    _end = false;
    std::cout<<"Hello World \n";
    if(argc < 2){
        fprintf(stderr, "ERROR, no port provided \n");
        exit(1);
    }
    int sockfd, portno;
    portno = atoi(argv[1]);
    
    sockfd = setupSocket(portno);
    int pid = fork();
    if(pid == 0){
        signal(SIGHUP, sighandle);
        listen(sockfd);
    }
    else if(pid>0){
        std::cout<<"Ending main process\n";
        signal(SIGINT, ctrlhandle);
        listenUdp();
        //sleep(60);
        kill(pid, SIGHUP);
    }
    else{
        std::cout<<"Something is not right here\n";
    }
    return 0;
}
