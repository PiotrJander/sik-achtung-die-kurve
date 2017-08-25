//
// Created by Piotr Jander on 23/08/2017.
//

#include "PlayerEliminatedEvent.h"

void PlayerEliminatedEvent::writeToBuffer(void *buffer)
{
    auto buf = reinterpret_cast<SelfPacked *>(buffer);
    *buf = SelfPacked(*this);
}

bool PlayerEliminatedEvent::operator==(const GameEvent &other) const
{
    if (auto *o = dynamic_cast<const PlayerEliminatedEvent *>(&other)) {
        return GameEvent::operator==(other) && o->playerNumber == playerNumber;
    } else {
        return false;
    }
}
