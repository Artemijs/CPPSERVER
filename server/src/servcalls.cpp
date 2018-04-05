#include <servcalls.h>
#include <iostream>
Server::Server():iServer(){}
Server::Server(int port): iServer(port, this) {
    _allPlayers = std::vector<Player>();
    _onTcpMessage = (&Server::onTcpMessage);
    //_onUdpMessage = (&Server::onUdpMessage);
    std::cout << &_onUdpMessage<<"\n";
    std::cout << &_onTcpMessage<<"\n";
}
/*  add another player to the list
    alert alert all existing players that a new player has connected
*/
void Server::handleTLogIn(char* name, int clientSock){
    //std::cout<<" THIS IS THE GOOD SHIT "<<buff<<"\n";
    Player newP;
    newP.tConn = clientSock; 
    int len = getMsgLen(name, 2);
    char buff[len];
    //the reason i start at 4 is because the first 2 chars are msg id next 2 are msg len
    for(int i = 4; i < len; i++){
        buff[i-4] = name[i];
    } 
    newP.name = std::string(buff);
    _allPlayers.push_back(newP);
    std::cout<<"Created player "<<_allPlayers.size()<<" "<<newP.name<<"\n";
    
}


void Server::onTcpMessage(  int clientSock, char* buff){
    std::cout<<"tcp message "<<buff <<"\n"; 
    //int id = getMsgId( buff);
    //std::cout << id<<"\n";
    //if(id == 0){
        //handleTLogIn(buff, clientSock);
    //}
}

void Server::onUdpMessage(char* buff, sockaddr_in clientAddr){
    std::cout<<"udp message \n";    
    //std::cout<<"udp message "<<buff <<"\n"; 

}

void Server::loadCallbacks(std::vector<function_ptr>* map){
    //maps onto ULOGIN
    //map->push_back((void(*)(char*))( &ServerCallbacks::handleTLogIn));
}

