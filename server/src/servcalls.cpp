#include <servcalls.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <unistd.h>

Server::Server():iServer(){}
Server::Server(int port): iServer(port, this) {
    _allPlayers = std::vector<Player>();
    _onTcpMessage = (&Server::onTcpMessage);
    _onUdpMessage = (&Server::onUdpMessage);
    std::cout << &_onUdpMessage<<"\n";
    std::cout << &_onTcpMessage<<"\n";
}
/*  add another player to the list
    alert alert all existing players that a new player has connected
*/
void Server::handleTLogIn(char* name, int clientSock){
    //std::cout<<" THIS IS THE GOOD SHIT "<<buff<<"\n";
    Player newP;
    bzero(&newP, sizeof(newP));
    newP.tConn = clientSock; 
    int len = getMsgLen(name, 2);
    char buff[len+1];
    memset(buff, 0x00, 4);
    //the reason i start at 4 is because the first 2 chars are msg id next 2 are msg len
    for(int i = 0; i < len; i++){
        buff[i] = name[i+4];
    } 
    newP.name = std::string(buff);
    newP.id = _allPlayers.size();
    _allPlayers.push_back(newP);
    std::cout<<"Created player "<<newP.id<<" "<<newP.name<<"\n";
    //send back id
    //std::string s = std::to_string(newP.id);
    char reply[5];
    memset(reply, 0x00, 5);
    char idstr[2];
    getLenStr(idstr, 0);
    reply[0] = idstr[0];
    reply[1] = idstr[1];
    getLenStr(idstr, newP.id);
    reply[2] = idstr[0];
    reply[3] = idstr[1];
   
    
    //printf("this is da wae %s \n", reply);
    //std::cout<<" the is is "+reply+"\n";
    int n = write(clientSock, reply, 4);  
    if( n < 0 ) error("ERROR write to socket\n");

    
}


void Server::onTcpMessage(  int clientSock, char* buff){
    std::cout<<"tcp message "<<buff <<"\n"; 

    int id = getMsgId( buff);
    std::cout << id<<"\n";
    if(id == 0){
        handleTLogIn(buff, clientSock);
    }
}

void Server::onUdpMessage(char* buff, sockaddr_in clientAddr){
    std::cout<<"udp message"<<buff<<" \n";    
    //std::cout<<"udp message "<<buff <<"\n"; 

}

void Server::loadCallbacks(std::vector<function_ptr>* map){
    //maps onto ULOGIN
    //map->push_back((void(*)(char*))( &ServerCallbacks::handleTLogIn));
}

