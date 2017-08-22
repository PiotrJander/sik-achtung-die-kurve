//
// Created by Piotr Jander on 22/08/2017.
//

#ifndef PROJECT_GAMEEVENT_H
#define PROJECT_GAMEEVENT_H

#include <iostream>


enum class EventType : uint8_t {
    NEW_GAME = 0,
    PIXEL = 1,
    PLAYER_ELIMINATED = 2,
    GAME_OVER = 3
};


class GameEvent {
public:
    GameEvent();

    GameEvent(uint32_t number, const EventType type);

    static std::unique_ptr<GameEvent>
    readFrom(std::istream &);

    void
    writeTo(std::ostream &);

    uint32_t
    getEventNumber() const;

    const EventType
    getType() const;

protected:
    virtual uint32_t
    getLength();

    virtual char *
    getData();

    virtual uint32_t
    getChecksum();

private:
    uint32_t eventNumber;
    const EventType type;
};


#endif //PROJECT_GAMEEVENT_H
