//
// Created by Piotr Jander on 26/08/2017.
//

#ifndef PROJECT_EVENTBATCH_H
#define PROJECT_EVENTBATCH_H


#include "IDatagram.h"
#include "Game.h"

class EventBatch: public IDatagram {
public:
    EventBatch(int length, const EventHistory &eventHistory, uint32_t startEventNo, uint32_t endEventNo,
               uint32_t gameId, size_t socketHash = EventBatch::BROADCAST)
            : length(length), socketHash(socketHash), eventHistory(eventHistory), 
              startEventNo(startEventNo), endEventNo(endEventNo), gameId(gameId)
    {}

    std::unique_ptr<char[]> getBuffer() override;

    int getLength() override
    {
        return length;
    }

    size_t getSocketHash() override
    {
        return socketHash;
    }

    static const int BROADCAST = 0;

private:
    int length;
    std::size_t socketHash;  // 0 means broadcast
    const EventHistory &eventHistory;
    uint32_t gameId;
    uint32_t startEventNo, endEventNo;  // start is inclusive, end is non-inclusive
};


#endif //PROJECT_EVENTBATCH_H
