//
// Created by Piotr Jander on 26/08/2017.
//

#ifndef PROJECT_EVENTBATCH_H
#define PROJECT_EVENTBATCH_H


#include "IDatagram.h"
#include "Game.h"

class EventBatch {
public:
    EventBatch(const Game &game, long begin, long end);

    DynamicBuffer getBuffer();

private:
    uint32_t gameId;
    std::vector<std::shared_ptr<GameEvent>> events;
};


#endif //PROJECT_EVENTBATCH_H
