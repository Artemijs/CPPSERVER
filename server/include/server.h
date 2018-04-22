#ifndef ISERVER
#define ISERVER
#include <vector>
#include <netinet/in.h>
class Server;
typedef void (Server::*function_ptr)(char*, struct sockaddr_in);
typedef void (Server::*function_ptr2)(char*, struct sockaddr_in, std::vector<function_ptr>*);
class iServer{
protected:
    int _udpSock;
    int _port;
    bool _end;
    //void loadHandles();
    function_ptr2 _onUdpMessage; 
    Server* _child;
public:
    iServer();
    iServer(int port, Server* childPtr);
    void udpSetUpSocket();
    void udpListen(std::vector<function_ptr>* ptr );
    int tcpAcceptConns();  
    void error(const char* msg);
    void end();
    void cleanUp();
    int getMsgId(char* buff);
    int getMsgLen(char* buff, int offset);
    void getLenStr(char* str, int len);
    void sendToAll(char* buff, int msgLen);
};
#endif
