//
// Created by Piotr Jander on 22/08/2017.
//

#ifndef PROJECT_EVENT_PIXEL_H
#define PROJECT_EVENT_PIXEL_H


#include "GameEvent.h"
#include "GameEventData.h"

#pragma pack(push, 1)
class PixelEventData: public GameEventData {
public:
    PixelEventData() = default;

    PixelEventData(uint8_t player_number, uint32_t x, uint32_t y)
            : player_number(player_number),
              x(htonl(x)),
              y(htonl(y))
    {}

    uint8_t getPlayer_number() const
    {
        return player_number;
    }

    uint32_t getX() const
    {
        return ntohl(x);
    }

    uint32_t getY() const
    {
        return ntohl(y);
    }

private:
    uint8_t player_number;
    uint32_t x, y;
};
#pragma pack(pop)


#endif //PROJECT_EVENT_PIXEL_H
