//
// Created by Piotr Jander on 23/08/2017.
//

#include "PlayerEliminatedEvent.h"

void PlayerEliminatedEvent::writeToBuffer(void *buffer)
{
    uint32_t length = getLength();
    auto buffer = std::make_unique<char[]>(length);
    std::memcpy(buffer.get(), &header, sizeof(header));
    std::memcpy(buffer.get() + sizeof(header), &playerNumber, sizeof(playerNumber));
    return buffer;
}

bool PlayerEliminatedEvent::operator==(const GameEvent &other) const
{
    if (auto *o = dynamic_cast<const PlayerEliminatedEvent *>(&other)) {
        return GameEvent::operator==(other) && o->playerNumber == playerNumber;
    } else {
        return false;
    }
}
