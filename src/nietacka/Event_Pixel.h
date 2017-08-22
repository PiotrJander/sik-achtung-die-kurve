//
// Created by Piotr Jander on 22/08/2017.
//

#ifndef PROJECT_EVENT_PIXEL_H
#define PROJECT_EVENT_PIXEL_H


#include "GameEvent.h"

class Event_Pixel: GameEvent {
private:
    const EventType type = EventType::PIXEL;
    uint8_t player_number;
    uint32_t x, y;
};


#endif //PROJECT_EVENT_PIXEL_H
