//
// Created by Piotr Jander on 24/08/2017.
//

#ifndef PROJECT_PLAYER_H
#define PROJECT_PLAYER_H


#include <string>
#include <sys/socket.h>
#include <map>
#include <netinet/in.h>
#include "gtest/gtest_prod.h"

class PlayerConnection {
public:
    FRIEND_TEST(GameTest, PlayerSessionIdChanges);

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

    PlayerConnection(const sockaddr *socketArg, uint64_t sessionId, int8_t turnDirection, const std::string &name)
            : sessionId(sessionId), turnDirection(turnDirection), name(name)
    {
        switch (socketArg->sa_family) {
            case AF_INET: {
                auto storage = reinterpret_cast<sockaddr_in *>(&socket);
                *storage = sockaddr_in(*reinterpret_cast<const sockaddr_in *>(socketArg));
            }
            case AF_INET6: {
                auto storage = reinterpret_cast<sockaddr_in6 *>(&socket);
                *storage = sockaddr_in6(*reinterpret_cast<const sockaddr_in6 *>(socketArg));
            }
        }
    }

    PlayerConnection(const PlayerConnection &pc)
            : sessionId(pc.sessionId), socket(pc.socket),
              turnDirection(pc.turnDirection), name(pc.name),
              readyForGame(pc.readyForGame),
              nextExpectedEvent(pc.nextExpectedEvent)
    {}

    size_t hash() const;

    static std::size_t getHashFor(const sockaddr *socket);

    void resetAfterGame();

    bool isReadyForGame() const
    {
        return readyForGame || name.empty();
    }

    void setTurnDirection(int8_t turnDirection)
    {
        PlayerConnection::turnDirection = turnDirection;
    }

    void setNextExpectedEvent(uint32_t nextExpectedEvent)
    {
        PlayerConnection::nextExpectedEvent = nextExpectedEvent;
    }

private:
    uint64_t sessionId;
    sockaddr_storage socket;
    int8_t turnDirection;
    std::string name;
    bool readyForGame = false;
    uint32_t nextExpectedEvent = 0;
};

typedef std::map<std::size_t, PlayerConnection> PlayerConnectionMap;

#endif //PROJECT_PLAYER_H
