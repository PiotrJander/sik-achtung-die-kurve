//
// Created by Piotr Jander on 23/08/2017.
//

#ifndef PROJECT_PIXELEVENT_H
#define PROJECT_PIXELEVENT_H


#include <utility>

#include "GameEvent.h"

class PixelEvent : public GameEvent {
public:
    struct DataPacked;
    struct SelfPacked;

    PixelEvent(GameEvent::HeaderPacked header, DataPacked data)
            : GameEvent(header), playerNumber(data.playerNumber), x(ntohl(data.x)), y(ntohl(data.y))
    {}

    PixelEvent(uint32_t eventNo, uint8_t player_number, uint32_t x, uint32_t y)
            : GameEvent(eventNo, GameEvent::Type::PIXEL), playerNumber(player_number), x(x), y(y)
    {}

    bool operator==(const GameEvent &other) const override;

    uint8_t getPlayerNumber() const
    {
        return playerNumber;
    }

    uint32_t getX() const
    {
        return x;
    }

    uint32_t getY() const
    {
        return y;
    }

private:
    uint8_t playerNumber;
    uint32_t x, y;

    void writeToBuffer(void *buffer) override;

    uint32_t getLength() override
    {
        return sizeof(SelfPacked);
    }
};

#pragma pack(push, 1)

struct PixelEvent::DataPacked {
    DataPacked(const PixelEvent &pixelEvent)
            : playerNumber(pixelEvent.getPlayerNumber()),
              x(htonl(pixelEvent.getX())),
              y(htonl(pixelEvent.getY()))
    {}

    uint8_t playerNumber;
    uint32_t x, y;
};

struct PixelEvent::SelfPacked {
    SelfPacked(const PixelEvent &pixelEvent) : header(pixelEvent), data(pixelEvent)
    {}

    GameEvent::HeaderPacked header;
    DataPacked data;
};


#pragma pack(pop)

#endif //PROJECT_PIXELEVENT_H
