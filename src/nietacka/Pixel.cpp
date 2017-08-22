//
// Created by Piotr Jander on 22/08/2017.
//

#include "Pixel.h"

//Pixel::Pixel(char *buffer)
//{
//    std::memcpy(this, buffer, sizeof(*this));
//}

uint8_t Pixel::getPlayer_number() const
{
    return player_number;
}

uint32_t Pixel::getX() const
{
    return x;
}

uint32_t Pixel::getY() const
{
    return y;
}

Pixel::Pixel(uint32_t number, uint8_t player_number, uint32_t x, uint32_t y)
        : GameEvent(number, EventType::PIXEL),
          player_number(player_number),
          x(x),
          y(y)
{}
