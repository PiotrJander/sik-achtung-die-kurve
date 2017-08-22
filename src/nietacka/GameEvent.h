//
// Created by Piotr Jander on 22/08/2017.
//

#ifndef PROJECT_GAMEEVENT_H
#define PROJECT_GAMEEVENT_H

#include <iostream>

class GameEvent {
public:
    void writeTo(std::ostream &);

private:
    uint32_t number;
    const EventType type;
};


enum class EventType : uint8_t {
    NEW_GAME = 0,
    PIXEL = 1,
    PLAYER_ELIMINATED = 2,
    GAME_OVER = 3
};


#endif //PROJECT_GAMEEVENT_H
