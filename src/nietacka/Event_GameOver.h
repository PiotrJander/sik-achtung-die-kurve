//
// Created by Piotr Jander on 22/08/2017.
//

#ifndef PROJECT_EVENT_GAMEOVER_H
#define PROJECT_EVENT_GAMEOVER_H


#include "GameEvent.h"

class Event_GameOver: GameEvent {
private:
    const EventType type = EventType::GAME_OVER;
};


#endif //PROJECT_EVENT_GAMEOVER_H
