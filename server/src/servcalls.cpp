#include <servcalls.h>
#include <iostream>
Server::Server():iServer(){}
Server::Server(int port): iServer(port){
    _allPlayers = std::vector<Player>();
    _onUdpMessage =(void(*)(char*, struct sockaddr_in))(&Server::onUdpMessage);
    _onTcpMessage = (void(*)(char*, int))(&Server::onTcpMessage);
}
/*  add another player to the list
    alert alert all existing players that a new player has connected
*/
void Server::handleTLogIn( int clientSock){
    //std::cout<<" THIS IS THE GOOD SHIT "<<buff<<"\n";
    Player newP;
    newP.tConn = clientSock; 
    _allPlayers.push_back(newP);
    
}


void Server::onTcpMessage(char* buff, int clientSock){}

void Server::onUdpMessage(char* buff, sockaddr_in clientAddr){}

void Server::loadCallbacks(std::vector<function_ptr>* map){
    //maps onto ULOGIN
    //map->push_back((void(*)(char*))( &ServerCallbacks::handleTLogIn));
}

