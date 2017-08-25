//
// Created by Piotr Jander on 23/08/2017.
//

#ifndef PROJECT_PIXELEVENT_H
#define PROJECT_PIXELEVENT_H


#include <utility>

#include "GameEvent.h"

class PixelEvent: public GameEvent {
public:

    #pragma pack(push, 1)
    class Data {
    public:
        Data() = default;

        Data(uint8_t player_number, uint32_t x, uint32_t y)
                : playerNumber(player_number),
                  x(htonl(x)),
                  y(htonl(y))
        {}

        uint8_t getPlayerNumber() const
        {
            return playerNumber;
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
        uint8_t playerNumber;
        uint32_t x, y;
    };
    #pragma pack(pop)

    PixelEvent(GameEvent::Header header, Data data) : GameEvent(header), data(data)
    {}

    PixelEvent(uint32_t eventNo, uint8_t player_number, uint32_t x, uint32_t y)
            : GameEvent(eventNo, GameEvent::Type::PIXEL), data(player_number, x, y)
    {}

    Data data;

    bool operator==(const GameEvent &other) const override;

private:
    void writeToBuffer(void *buffer) override;

    uint32_t getLength() override
    {
        return sizeof(header) + sizeof(data);
    }
};


#endif //PROJECT_PIXELEVENT_H
