#include <vector>
#include "servcalls.h"
class Server{
private:
    int _udpSock;
    int _tcpSock;
    int _port;
    bool _end;
    ServerCallbacks _callHandler;
    std::vector<function_ptr> _allMsgHandles;
    void loadHandles();
    
public:
    Server();
    Server(int port);
    void tcpSetUpSocket();
    void udpSetUpSocket();
    void tcpListen();
    void udpListen();
    void tcpHandleConns(int clientSock);  
    int tcpAcceptConns();  
    void error(const char* msg);
    void end();
    void cleanUp();
    int getMsgId(char* buff);
};

