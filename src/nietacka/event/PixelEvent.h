//
// Created by Piotr Jander on 23/08/2017.
//

#ifndef PROJECT_PIXELEVENT_H
#define PROJECT_PIXELEVENT_H


#include <utility>

#include "GameEvent.h"
#include "../easylogging++.h"

class PixelEvent : public GameEvent {
public:

    /*
     * packed structs ~~~~~~~~~~~~~~~~~~~~~
     */
#pragma pack(push, 1)
    struct DataPacked {
        DataPacked(const PixelEvent &pixelEvent)
                : playerNumber(pixelEvent.getPlayerNumber()),
                  x(htonl(pixelEvent.getX())),
                  y(htonl(pixelEvent.getY()))
        {}

        uint8_t playerNumber;
        uint32_t x, y;
    };

    struct SelfPacked {
        SelfPacked(const PixelEvent &pixelEvent) : header(pixelEvent), data(pixelEvent)
        {}

        HeaderPacked header;
        DataPacked data;
    };
#pragma pack(pop)
    /*
     * END packed structs ~~~~~~~~~~~~~~~~~~~~~~
     */

    PixelEvent(const SelfPacked &packed)
            : GameEvent(packed.header),
              playerNumber(packed.data.playerNumber),
              x(ntohl(packed.data.x)),
              y(ntohl(packed.data.y))
    {}

    PixelEvent(uint32_t eventNo, uint8_t player_number, uint32_t x, uint32_t y)
            : GameEvent(eventNo, Type::PIXEL), playerNumber(player_number), x(x), y(y)
    {
        LOG(INFO) << "PixelEvent";
    }

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

    void writeSelf(DynamicBuffer &buffer) override;

private:
    uint8_t playerNumber;
    uint32_t x, y;

    void writeToBuffer(char *buffer) override;

    uint32_t selfLength() const override
    {
        return sizeof(SelfPacked);
    }
};

#endif //PROJECT_PIXELEVENT_H
