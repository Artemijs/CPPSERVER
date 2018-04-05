#include "server.h"
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
    std::cout<<"online\n";
    int pid = fork();
    if(pid == 0){
        signal(SIGHUP, sighandle);
        _server.tcpListen();    
    }
    else if (pid>0){
        signal(SIGINT, ctrlhandle);
        _server.udpListen();
        //end process of the tcp listener
        //only happens when udp stops listening which only happens when ctrl c is pressed
        kill(pid, SIGHUP);
    }
    else{
        std::cout<<"SOMETHING IS NOT RIGHT HERE\n";
    }
    _server.cleanUp();
    return 0;
}
