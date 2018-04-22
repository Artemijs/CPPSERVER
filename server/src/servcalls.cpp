#include <servcalls.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <unistd.h>

Server::Server():iServer(){
    
}
Server::Server(int port): iServer(port, this) {
    _allPlayers = std::vector<Player>();
    _onUdpMessage = (&Server::onUdpMessage);

    _allMsgHandles =  std::vector<function_ptr>(); 

    _allMsgHandles.push_back( &Server::handleLogIn);
    _allMsgHandles.push_back( &Server::handleLoadPlayers);
    _allMsgHandles.push_back( &Server::removePlayer);
    _allMsgHandles.push_back( &Server::chatMsg);
    _allMsgHandles.push_back( &Server::handlePosition);
}
Server::~Server(){
}

void Server::sendToAllExcept(const char* buff, int msgLen, struct Player player){
    int fromlen = sizeof(struct sockaddr_in);
    for(unsigned int i =0; i <_allPlayers.size(); ++i){
        
        if(player.id != i){
            int n = sendto(_udpSock, buff, msgLen, 0, (struct sockaddr*)&_allPlayers[i].uConn, fromlen);
            if(n < 0)error("sendto");
        }
    }

}
void Server::sendToAllExcept(const char* buff, int msgLen, sockaddr_in exception){
    int fromlen = sizeof(struct sockaddr_in);
    for(unsigned int i =0; i <_allPlayers.size(); ++i){
        
        if(exception.sin_addr.s_addr != _allPlayers[i].uConn.sin_addr.s_addr){
            int n = sendto(_udpSock, buff, msgLen, 0, (struct sockaddr*)&_allPlayers[i].uConn, fromlen);
            if(n < 0)error("sendto");
        }
    }
}

void Server::sendToAll(char* buff, int msgLen){
    int fromlen = sizeof(struct sockaddr_in);
    for(unsigned int i =0; i <_allPlayers.size(); ++i){
        int n = sendto(_udpSock, buff, msgLen, 0, (struct sockaddr*)&_allPlayers[i].uConn, fromlen);
        if(n < 0)error("sendto");

    }
}

void Server::handlePosition(char* buff, sockaddr_in client){
    int len = getMsgLen(buff, 2);
    sendToAll(buff, len+4); 
}

void Server::chatMsg(char* buff, sockaddr_in client){
    int len = getMsgLen(buff, 2);
    std::cout<<"chat msg : "<<buff<<" "<<len<<"\n";
    sendToAll(buff, len+4); 
}

void Server::removePlayer(char* buff, sockaddr_in client){
    //buff = 00id 00 len [index of player]
    int id = getMsgLen(buff, 4);
    //char* b2 = &buff[2];
    //int id = atoi(b2);
    std::cout<<"REMOVING PLAYER "<<id<<"/"<<_allPlayers.size()<<"  "<<buff<<"\n";
    Player p = _allPlayers[id];
    int len = _allPlayers.size();
    if(p.id != id){
        id = -1;
        for(int i =0; i < len; ++i){
            if(client.sin_addr.s_addr == _allPlayers[i].uConn.sin_addr.s_addr){
                id = i;
            }
        } 
        if(id == -1) return;
    }
    _allPlayers.erase(_allPlayers.begin()+id);
    //this needs to inform others that a player has left
    char str[5];
    memset(str, 0x00, 5);
    getLenStr(&(str[0]), PLAYER_LEFT);
    getLenStr(&(str[2]), p.id);     
    sendToAll(str, 4);
}

void Server::handleLoadPlayers(char* buff, sockaddr_in client){
    //serialize all players
    std::string s;
    char buf[2];
    getLenStr(buf, LOADPLAYERS);
    
    for(unsigned int i =0; i < _allPlayers.size(); ++i){
        //this shold be a function called serialise player or somethin
        s+=_allPlayers[i].name+"," + std::to_string(_allPlayers[i].id) + "|";
    }
    s = std::string(buf)+s;
    std::cout<<"Sending :"<<s<<"\n";
    int n = sendto(_udpSock, s.c_str(), s.length(), 0, (struct sockaddr*) &client, sizeof(struct sockaddr_in));
    if( n < 0 ) error("ERROR write to socket\n");
    
}

/*  add another player to the list
    send back player id to the client
*/
void Server::handleLogIn(char* name, sockaddr_in client){
    Player newP;
    bzero(&newP, sizeof(newP));

    int len = getMsgLen(name, 2);
    char buff[len+1];
    memset(buff, 0x00, len+1);

    //the reason i start at 4 is because the first 2 chars are msg id next 2 are msg len
    for(int i = 0; i < len; i++){
        buff[i] = name[i+4];
    } 

    newP.name = std::string(buff);
    newP.id = _allPlayers.size();
    newP.uConn = client;

    _allPlayers.push_back(newP);
    std::cout<<"Created player "<<newP.id<<" "<<newP.name<<"\n";
    //send back id
    //std::string s = std::to_string(newP.id);
    char reply[5];
    memset(reply, 0x00, 5);
    char idstr[2];
    getLenStr(idstr, LOGIN);
    reply[0] = idstr[0];
    reply[1] = idstr[1];
    getLenStr(idstr, newP.id);
    reply[2] = idstr[0];
    reply[3] = idstr[1];
   
    
    //printf("this is da wae %s \n", reply);
    //std::cout<<" the is is "+reply+"\n";

    int n = sendto(_udpSock, reply, 4, 0, (struct sockaddr*) &client, sizeof(struct sockaddr_in));
    //int n = write(clientSock, reply, 4);  
    if( n < 0 ) error("ERROR write to socket\n");
   //this needs to inform others that a new player has joined 
    char buf[3];
    memset(buf, 0x00, 3);
    getLenStr(buf, LOADPLAYERS);
    std::string s = std::string(buf) + newP.name+"," + std::to_string(newP.id) + "|";
    std::cout<<newP.name<<"\n";
    std::cout<<"sending to all "<<s<<"\n";
    sendToAllExcept(s.c_str(), s.length()+2, newP);
}



void Server::onUdpMessage(char* buff, sockaddr_in clientAddr, std::vector<function_ptr>* ptr){
    //std::cout<<"udp message "<<buff <<"\n"; 
    int id = getMsgId(buff);
    if(id >= END_OF_NUM){
        std::cout<<"udp message out of bounds "<<"\n";    
        return;
    }
    //handleLogIn(buff, clientAddr);
    (this->*(*ptr)[id])(buff, clientAddr);
}

void Server::loadCallbacks(){
    //maps onto LOGIN = 0
    _allMsgHandles.push_back( &Server::handleLogIn);
    std::cout<<_allMsgHandles.size()<<"\n";
}

std::vector<void(Server::*)(char*, sockaddr_in)>* Server::getHandles(){
    return &_allMsgHandles;
}
