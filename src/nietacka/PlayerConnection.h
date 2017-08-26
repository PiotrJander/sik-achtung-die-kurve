//
// Created by Piotr Jander on 24/08/2017.
//

#ifndef PROJECT_PLAYER_H
#define PROJECT_PLAYER_H


#include <string>
#include <sys/socket.h>
#include <map>

class PlayerConnection {
public:
    const std::string &getName() const
    {
        return name;
    }

    uint64_t getSessionId() const
    {
        return sessionId;
    }

    int8_t getTurnDirection() const
    {
        return turnDirection;
    }

    PlayerConnection(uint64_t sessionId, int socket, int8_t turnDirection, const std::string &name)
            : socket(socket), sessionId(sessionId), turnDirection(turnDirection), name(name)
    {}

    static std::size_t getHashFor(sockaddr *structsockaddr);

private:
    uint64_t sessionId;
    int socket;
    int8_t turnDirection;
    std::string name;
};

typedef std::map<std::size_t, PlayerConnection> PlayerConnectionMap;

#endif //PROJECT_PLAYER_H
