//
// Created by Piotr Jander on 26/08/2017.
//

#include "EventBatch.h"

DynamicBuffer EventBatch::getBuffer()
{
    DynamicBuffer buffer;
    buffer << gameId;

    for (auto &&item : events) {
        item->write(buffer);
    }

    return buffer;
}

EventBatch::EventBatch(const Game &game, long begin, long end) : gameId(game.getId()), events()
{
    std::copy(game.getEventHistory().begin() + begin, game.getEventHistory().begin() + end, std::back_inserter(events));
}
