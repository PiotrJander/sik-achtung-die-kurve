//
// Created by Piotr Jander on 23/08/2017.
//

#ifndef PROJECT_GAMEOVEREVENT_H
#define PROJECT_GAMEOVEREVENT_H


#include "GameEvent.h"

class GameOverEvent: public GameEvent {
public:
    explicit GameOverEvent(const Header &header) : GameEvent(header)
    {}

    explicit GameOverEvent(uint32_t eventNo) : GameEvent(eventNo, Type::GAME_OVER)
    {}

private:
    uint32_t getLength() override
    {
        return sizeof(header);
    }

    std::unique_ptr<char[]> getBuffer() override;
};


#endif //PROJECT_GAMEOVEREVENT_H