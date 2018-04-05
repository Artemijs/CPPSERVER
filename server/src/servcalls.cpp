#include <servcalls.h>
#include <iostream>
ServerCallbacks::ServerCallbacks(){}
ServerCallbacks::ServerCallbacks(int udp, int tcp):_udpSock(udp), _tcpSock(tcp){
    _allPlayers = std::vector<Player>();
}
/*  add another player to the list
    alert alert all existing players that a new player has connected
*/
void ServerCallbacks::handleTLogIn( int clientSock){
    std::cout<<" THIS IS THE GOOD SHIT "<<buff<<"\n";
    Player newP;
    newP.tConn = clientSock; 
    _allPlayers.push_back(newP);
    
}
void ServerCallbacks::loadCallbacks(std::vector<function_ptr>* map){
    //maps onto ULOGIN
    //map->push_back((void(*)(char*))( &ServerCallbacks::handleTLogIn));
}

