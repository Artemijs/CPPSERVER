#ifndef ISERVER
#define ISERVER
#include <vector>
#include <netinet/in.h>
typedef void (*function_ptr)(char*);
//typedef void (*msgCallBackTcp)(int);
class Server;
typedef void (Server::*msgCallBackTcp)(int, char*);
class iServer;
typedef void (iServer::*msgCallBackUdp)(int);
class iServer{
protected:
    int _udpSock;
    int _tcpSock;
    int _port;
    bool _end;
    std::vector<function_ptr> _allMsgHandles;
    //void loadHandles();
    msgCallBackTcp _onTcpMessage;
    msgCallBackUdp _onUdpMessage; 
    char* _tcpMsg; 
    Server* _child;
public:
    iServer();
    iServer(int port, Server* childPtr);
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
    int getMsgLen(char* buff, int offset);
    void hello(int );
};
#endif
