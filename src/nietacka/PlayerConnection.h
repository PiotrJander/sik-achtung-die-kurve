//
// Created by Piotr Jander on 24/08/2017.
//

#ifndef PROJECT_PLAYER_H
#define PROJECT_PLAYER_H


#include <string>

class PlayerConnection {
public:
    const std::string &getName() const
    {
        return name;
    }

    int getSessionId() const
    {
        return sessionId;
    }

    uint8_t getTurnDirection() const
    {
        return turnDirection;
    }

private:
    int socket, sessionId;
    uint8_t turnDirection;
    std::string name;
};


#endif //PROJECT_PLAYER_H
