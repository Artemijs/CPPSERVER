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
    _allOffline = std::vector<Player>();

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
    //find the clients player and update his position
    int id = getMsgLen(buff, 4);   
    id = checkId(id, client);
    Player p = _allPlayers[id];
    /*char pos[len-4];
    memset(pos, 0x00, len-4);
    (pos[0]) = (*buff[4]);*/
    p.position = std::string(&(buff[7]));
    _allPlayers[id] = p;
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
    Player p = _allPlayers[id];
    std::cout<<"REMOVING PLAYER "<<id<<"/"<<_allPlayers.size()<<"  "<<buff<<"\n";
    /*int len = _allPlayers.size();
    if(p.id != id){
        id = -1;
        for(int i =0; i < len; ++i){
            if(client.sin_addr.s_addr == _allPlayers[i].uConn.sin_addr.s_addr){
                id = i;
            }
        } 
        if(id == -1) return;
    }*/
    id = checkId(id, client);
    if(id < 0){
        std::cout<<"INVALID ID \n";
        return;
    }
    int len = _allPlayers.size();
    _allPlayers.erase(_allPlayers.begin()+id);
    for(int i =0; i < len; ++i){
        _allPlayers[i].id = i;
    }
    //this needs to inform others that a player has left
    char str[5];
    memset(str, 0x00, 5);
    getLenStr(&(str[0]), PLAYER_LEFT);
    getLenStr(&(str[2]), p.id);     
    sendToAll(str, 4);
    _allOffline.push_back(p);
}

void Server::handleLoadPlayers(char* buff, sockaddr_in client){
    //serialize all players
    std::string s;
    char buf[2];
    getLenStr(buf, LOADPLAYERS);
    
    for(unsigned int i =0; i < _allPlayers.size(); ++i){
        //this shold be a function called serialise player or somethin
        s += serializePlayer(_allPlayers[i]);
        //_allPlayers[i].name+"," + std::to_string(_allPlayers[i].id) + "|";
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
    
    //getMsgLen does not include the lenght of the header which is 4
    int len = getMsgLen(name, 2) + 4;

    char nameBuff[len];
    memset(nameBuff, 0x00, len);
    char loginBuff[len];
    memset(loginBuff, 0x00, len);
    char reply[5];
    memset(reply, 0x00, 5);

    //the reason i start at 4 is because the first 2 chars are msg id next 2 are msg len
    int i = 4;
    for(; i < len; i++){
        if(name[i] == '|') break;
        nameBuff[i-4] = name[i];
    }
    //remove the '|'
    i++;
    int l = len - i;
    for(int j = 0; j < l; j++){
        loginBuff[j] = name[i];
        i++;
    }
    int offline = checkIfOffline(loginBuff);
    int online = checkIfOnline(loginBuff);
    //if online throw error
    //if offline login
    if(online >= 0){
        std::cout<<"Player already EXISTS "<<loginBuff<<"\n";
        getLenStr(reply, LOGIN);
        reply[2]='-';
        reply[3] = '1';
        int n = sendto(_udpSock, reply, 4, 0, (struct sockaddr*) &client, sizeof(struct sockaddr_in));
        if( n < 0 ) error("ERROR write to socket\n");
        return;
    }
    if(offline >= 0){
        newP = _allOffline[offline];
        newP.id = _allPlayers.size();
        _allOffline.erase(_allOffline.begin()+offline);

        std::cout<<"Player LOADED "<<loginBuff<<newP.position<<"\n";
    }
    else{
        newP.name = std::string(nameBuff);
        newP.id = _allPlayers.size();
        newP.uConn = client;
        newP.login = std::string(loginBuff);
        newP.position = "0,0,0#0,0,0";

        std::cout<<"Created player "<<newP.id<<" "<<newP.name<<"  "<<newP.login<<"\n";
    }
    _allPlayers.push_back(newP);

    //send back id
    getLenStr(reply, LOGIN);
    getLenStr(&(reply[2]), newP.id);
   
    int n = sendto(_udpSock, reply, 4, 0, (struct sockaddr*) &client, sizeof(struct sockaddr_in));
    if( n < 0 ) error("ERROR write to socket\n");

   //this informs others that a new player has joined 
    std::string s = serializePlayer(LOADPLAYERS, newP);
    std::cout << s<<"\n";
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

int Server::checkIfOnline(char* login){
    for(unsigned int i = 0; i < _allPlayers.size(); ++i){
        if(_allPlayers[i].login.compare(login) == 0){
            return i;
        }  
    }
    return -1;
}

int Server::checkIfOffline(char* login){
    for(unsigned int i = 0; i < _allOffline.size(); ++i){
        if(_allOffline[i].login.compare(login) == 0){
            return i;
        }  
    } 
    return -1;
}



std::string Server::serializePlayer(int msgType,struct Player p){
    char b[2]; 
    getLenStr(b, msgType);
    return std::string(b) + p.name+"," + std::to_string(p.id) + "#" + p.position + "|";
}
std::string Server::serializePlayer(struct Player p){
    
    return p.name+"," + std::to_string(p.id) + "#" + p.position + "|";
}

int Server::checkId(int id, struct sockaddr_in pAddr){
    if(pAddr.sin_addr.s_addr == _allPlayers[id].uConn.sin_addr.s_addr){
        return id;
    }
    for(unsigned int i =0; i < _allPlayers.size(); ++i){
        if(pAddr.sin_addr.s_addr == _allPlayers[i].uConn.sin_addr.s_addr){
            id = i;
            return id;
        }
    }
    return -1;
}
