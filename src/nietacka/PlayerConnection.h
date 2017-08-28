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
    FRIEND_TEST(GameManagerTest, CanGameStart);

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

    PlayerConnection(const sockaddr *socketArg, uint64_t sessionId, int8_t turnDirection, const std::string &name);

    PlayerConnection(const PlayerConnection &pc)
            : sessionId(pc.sessionId), socketStorage(pc.socketStorage),
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

    uint32_t getNextExpectedEvent() const
    {
        return nextExpectedEvent;
    }

    void setTurnDirection(int8_t turnDirection)
    {
        if (turnDirection == -1 || turnDirection == 1) {
            readyForGame = true;
        }
        PlayerConnection::turnDirection = turnDirection;
    }

    void setNextExpectedEvent(uint32_t nextExpectedEvent)
    {
        PlayerConnection::nextExpectedEvent = nextExpectedEvent;
    }

    const sockaddr_storage &getSocketStorage() const
    {
        return socketStorage;
    }

private:
    uint64_t sessionId;
    sockaddr_storage socketStorage;
    int8_t turnDirection;
    std::string name;
    bool readyForGame = false;
    uint32_t nextExpectedEvent = 0;
};

typedef std::map<std::size_t, PlayerConnection> PlayerConnectionMap;

#endif //PROJECT_PLAYER_H
