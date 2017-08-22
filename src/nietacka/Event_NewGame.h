//
// Created by Piotr Jander on 22/08/2017.
//

#ifndef PROJECT_EVENT_NEWGAME_H
#define PROJECT_EVENT_NEWGAME_H

#include "GameEvent.h"

class Event_NewGame: GameEvent {
private:
    Event_NewGame() {}

    const EventType type = EventType::NEW_GAME;
    uint32_t maxx, maxy;
    // TODO player names
};


#endif //PROJECT_EVENT_NEWGAME_H
