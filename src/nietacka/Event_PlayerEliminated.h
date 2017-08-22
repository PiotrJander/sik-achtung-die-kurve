//
// Created by Piotr Jander on 22/08/2017.
//

#ifndef PROJECT_EVENT_PLAYERELIMINATED_H
#define PROJECT_EVENT_PLAYERELIMINATED_H


#include "GameEvent.h"

class Event_PlayerEliminated: GameEvent {
private:
    Event_PlayerEliminated() {}

    const EventType type = EventType::PLAYER_ELIMINATED;
    uint8_t player_number;
};


#endif //PROJECT_EVENT_PLAYERELIMINATED_H
