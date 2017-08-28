//
// Created by Piotr Jander on 26/08/2017.
//

#ifndef PROJECT_EVENTBATCH_H
#define PROJECT_EVENTBATCH_H


#include "IDatagram.h"
#include "Game.h"

class EventBatch {
public:
    EventBatch(int length, const EventHistory &eventHistory, 
               uint32_t startEventNo, uint32_t endEventNo, uint32_t gameId)
            : length(length), eventHistory(eventHistory),
              startEventNo(startEventNo), endEventNo(endEventNo), gameId(gameId)
    {}

    DynamicBuffer getBuffer();

private:
    int length;
    const EventHistory &eventHistory;
    uint32_t gameId;
    uint32_t startEventNo, endEventNo;  // start is inclusive, end is non-inclusive
};


#endif //PROJECT_EVENTBATCH_H
