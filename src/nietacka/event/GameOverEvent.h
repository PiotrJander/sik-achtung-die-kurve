//
// Created by Piotr Jander on 23/08/2017.
//

#ifndef PROJECT_GAMEOVEREVENT_H
#define PROJECT_GAMEOVEREVENT_H


#include "../DynamicBuffer.h"
#include "GameEvent.h"
#include "../easylogging++.h"

class GameOverEvent: public GameEvent {
public:
    explicit GameOverEvent(const HeaderPacked &header) : GameEvent(header)
    {}

    explicit GameOverEvent(uint32_t eventNo) : GameEvent(eventNo, Type::GAME_OVER)
    {
        LOG(INFO) << "GameOverEvent";
    }

    void writeSelf(DynamicBuffer &buffer) override;


private:
    uint32_t selfLength() const override
    {
        return sizeof(HeaderPacked);
    }
};


#endif //PROJECT_GAMEOVEREVENT_H
