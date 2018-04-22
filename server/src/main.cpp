#include "servcalls.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
Server _server;

void sighandle(int signo){
    std::cout<<"Got a signal : "<<signo<<std::endl;
    exit(signo);
}
void ctrlhandle(int signo){
    std::cout<<"Got a signal : "<<signo<<std::endl;
    _server.end();
}
int main(int argc, char* argv[]){
    if(argc < 2){
        fprintf(stderr, "ERROR, no port provided \n");
        exit(1);
    }
    std::cout<<"Starting server \n";
    _server = Server(atoi(argv[1]));
    std::vector<void(Server::*)(char*, sockaddr_in)>* ptr = _server.getHandles();
    std::cout<<"online\n";
    signal(SIGINT, ctrlhandle);
    _server.udpListen( ptr);
    _server.cleanUp();
    return 0;
}
