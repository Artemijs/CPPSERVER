#include <netinet/in.h>
#include "server.h"
enum NetCalls{
    ULOGIN=0,
    TLOGIN=1,
    END
};
struct Player{
    sockaddr_in uConn;
    int tConn;
};
class Server : public iServer{
private:
    std::vector<Player> _allPlayers;
public:
    Server();
    Server(int port);
    //void tSend(int sock, char* buff, msgLen);
    void handleTLogIn(int newConn); 
    void loadCallbacks(std::vector<function_ptr>* arr);
    void onTcpMessage(char* buff, int clientSock);
    void onUdpMessage(char* buff, sockaddr_in clientAddr);
    //void uSendToAll(sockaddr_in* all_conns, int connCount, int sock, char* buff, int msgLen);


};
