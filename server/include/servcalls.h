#ifndef SERVER
#define SERVER
#include <string>
#include "server.h"

enum NetCalls{
    LOGIN=0,
    END
};

struct Player{
    sockaddr_in uConn;
    int tConn;
    std::string name;
    int id;
};

class Server : public iServer{
private:
    std::vector<Player> _allPlayers;
public:
    Server();
    Server(int port);
    //void tSend(int sock, char* buff, msgLen);
    void handleTLogIn(char* buff, int newConn); 
    void loadCallbacks(std::vector<function_ptr>* arr);
    void onTcpMessage( int clientSock, char* buff);
    void onUdpMessage(char* buff, sockaddr_in clientAddr);
    //void uSendToAll(sockaddr_in* all_conns, int connCount, int sock, char* buff, int msgLen);

};
#endif
