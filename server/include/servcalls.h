#include <vector>
#include <netinet/in.h>

enum NetCalls{
    ULOGIN=0,
    TLOGIN=1,
    END
};
typedef void (*function_ptr)(char*);
struct Player{
    sockaddr_in uConn;
    int tConn;
};
class ServerCallbacks{
private:
    int _udpSock;
    int _tcpSock;
    std::vector<Player> _allPlayers;
public:
    ServerCallbacks();
    ServerCallbacks(int udp, int tcp);
    void tSend(int sock, char* buff, msgLen);
    void handleTLogIn(int newConn); 
    void loadCallbacks(std::vector<function_ptr>* arr);
    //void uSendToAll(sockaddr_in* all_conns, int connCount, int sock, char* buff, int msgLen);


};
