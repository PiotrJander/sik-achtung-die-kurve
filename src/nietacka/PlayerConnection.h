//
// Created by Piotr Jander on 24/08/2017.
//

#ifndef PROJECT_PLAYER_H
#define PROJECT_PLAYER_H


#include <string>
#include <sys/socket.h>
#include <map>
#include <netinet/in.h>

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

    PlayerConnection(sockaddr *socketArg, uint64_t sessionId, int8_t turnDirection, const std::string &name)
            : sessionId(sessionId), turnDirection(turnDirection), name(name)
    {
        switch (socketArg->sa_family) {
            case AF_INET: {
                auto storage = reinterpret_cast<sockaddr_in *>(&socket);
                *storage = sockaddr_in(*reinterpret_cast<sockaddr_in *>(socketArg));
            }
            case AF_INET6: {
                auto storage = reinterpret_cast<sockaddr_in6 *>(&socket);
                *storage = sockaddr_in6(*reinterpret_cast<sockaddr_in6 *>(socketArg));
            }
        }
    }

    size_t hash() const;

private:
    uint64_t sessionId;
    sockaddr_storage socket;
    int8_t turnDirection;
    std::string name;
};

typedef std::map<std::size_t, PlayerConnection> PlayerConnectionMap;

#endif //PROJECT_PLAYER_H
