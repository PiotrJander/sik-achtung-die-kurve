//
// Created by Piotr Jander on 22/08/2017.
//

#ifndef PROJECT_EVENT_PIXEL_H
#define PROJECT_EVENT_PIXEL_H


#include "GameEvent.h"

class Pixel: public GameEvent {
public:
    Pixel(uint32_t number, uint8_t player_number, uint32_t x, uint32_t y);

//    explicit Pixel(char *buffer);

    uint8_t getPlayer_number() const;

    uint32_t getX() const;

    uint32_t getY() const;

private:
    uint8_t player_number;
    uint32_t x, y;
};


#endif //PROJECT_EVENT_PIXEL_H
