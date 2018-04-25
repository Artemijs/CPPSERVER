#ifndef SERVER
#define SERVER
#include <string>
#include "server.h"

enum NetCalls{
    LOGIN=0,
    LOADPLAYERS,
    REMOVEPLAYER,
    MSG,
    POSITION,
    PLAYER_LEFT,
    END_OF_NUM
};

struct Player{
    sockaddr_in uConn;
    std::string name;
    std::string login;
    int id;
    std::string position; 
};

class Server : public iServer{
private:
    std::vector<Player> _allPlayers;
    std::vector<Player> _allOffline;
    std::vector<function_ptr> _allMsgHandles;
public:
    Server();
    Server(int port);
    ~Server();
    /*
        senDS a message to every player in _allPlayers
        buff : the message to be transfered
        msgLen : lenght of message
    */
    void sendToAll( char* buff, int msgLen);
    /*
        sends a message to all but exception sockaddr
    */
    void sendToAllExcept(const char* buff, int msgLen, sockaddr_in exception);
    /*
        sends a message to all but player 
    */
    void sendToAllExcept(const char* buff, int msgLen, struct Player player);
    /*
        handlesmessages from clients
        uses sendToAll
    */
    void chatMsg(char* buff, sockaddr_in client);
    /*
       serializes _allPlayers and sends serialized data back to client
    */
    void handleLoadPlayers(char* buff, sockaddr_in client);
    /*
        resends data from client
        uses sendToAll 
    */
    void handlePosition(char* buff, sockaddr_in client);
    /*
        removes from _allPlayers by setting everything after id to current = next and setting player.id = i - 1
        sends to all PLAYER_LEFT00id 
    */
    void removePlayer(char* buff, sockaddr_in clientAddr); 
    /*
        checks if player already exists
        creates a new Player struct, adds it to _allPlayers and returns ID to client
        buff : contains name of new player
        players id is its position in _allPlayers
        sendToAll LOAD_PLAYERS serialized new player
    */ 
    void handleLogIn(char* buff, sockaddr_in clientAddr); 
    /*
        loads all of the functions into memory
    */
    void loadCallbacks();
    /*
        called from base class via the function_ptr2 variable
    */
    void onUdpMessage(char* buff, sockaddr_in clientAddr, std::vector<function_ptr>* ptr);
    /*
        DO NOT TOUCH THIS PRECIOUS THING
    */
    std::vector<function_ptr>* getHandles();
        
    std::string serializePlayer(int msgType, struct Player p);
    std::string serializePlayer( struct Player p);
    /*
        checks if id matches with _allPlayers[id].id and if not 
        finds the player is does match with
        retruns -1 if nothing found
    */
    int checkId(int id, struct sockaddr_in);
    
    int checkIfOnline(char* login);
    int checkIfOffline(char* login);
};
#endif
