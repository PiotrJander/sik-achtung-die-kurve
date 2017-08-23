//
// Created by Piotr Jander on 23/08/2017.
//

#ifndef PROJECT_PIXELEVENT_H
#define PROJECT_PIXELEVENT_H


#include <utility>

#include "GameEvent.h"
#include "PixelEventData.h"

class PixelEvent: public GameEvent {
public:
    PixelEvent(GameEventHeader header, PixelEventData data) : GameEvent(header), data(std::move(data))
    {}

    PixelEvent(uint32_t eventNo, uint8_t player_number, uint32_t x, uint32_t y)
            : GameEvent(eventNo, EventType::PIXEL), data(player_number, x, y)
    {}

private:
    std::unique_ptr<char[]> getBuffer() override;

    uint32_t getLength() override;

    PixelEventData data;
};


#endif //PROJECT_PIXELEVENT_H
