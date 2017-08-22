//
// Created by Piotr Jander on 22/08/2017.
//

#include "Event_Pixel.h"

Event_Pixel::Event_Pixel(char *buffer)
{
    std::memcpy(this, buffer, sizeof(*this));
}

uint8_t Event_Pixel::getPlayer_number() const
{
    return player_number;
}

uint32_t Event_Pixel::getX() const
{
    return htonl(x);
}

uint32_t Event_Pixel::getY() const
{
    return htonl(y);
}

Event_Pixel::Event_Pixel(uint32_t number, uint8_t player_number, uint32_t x, uint32_t y)
        : GameEvent(number, EventType::PIXEL),
          player_number(player_number),
          x(htonl(x)),
          y(htonl(y))
{}
