//
// Created by Piotr Jander on 22/08/2017.
//

#ifndef PROJECT_GAMEEVENT_H
#define PROJECT_GAMEEVENT_H

#include <iostream>
#include "GameEventHeader.h"


class GameEvent {
public:
    virtual ~GameEvent() = default;

    explicit GameEvent(GameEventHeader header) : header(std::move(header))
    {}

    GameEvent(uint32_t eventNo, EventType type) : header(eventNo, type)
    {}

    static std::unique_ptr<GameEvent>
    readFrom(std::istream &);

    void
    writeTo(std::ostream &);

    GameEventHeader header;

private:
    virtual uint32_t getLength() = 0;

    virtual std::unique_ptr<char[]> getBuffer() = 0;
};


#endif //PROJECT_GAMEEVENT_H
