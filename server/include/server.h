#include <vector>
#include <netinet/in.h>
typedef void (*function_ptr)(char*);
typedef void (*msgCallBackTcp)(char*, int);
typedef void (*msgCallBackUdp)(char*, sockaddr_in);
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
public:
    iServer();
    iServer(int port);
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

