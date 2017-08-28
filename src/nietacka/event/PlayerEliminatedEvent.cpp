//
// Created by Piotr Jander on 23/08/2017.
//

#include "PlayerEliminatedEvent.h"

bool PlayerEliminatedEvent::operator==(const GameEvent &other) const
{
    if (auto *o = dynamic_cast<const PlayerEliminatedEvent *>(&other)) {
        return GameEvent::operator==(other) && o->playerNumber == playerNumber;
    } else {
        return false;
    }
}

void PlayerEliminatedEvent::writeSelf(DynamicBuffer &buffer)
{
    SelfPacked packed(*this);
    buffer << packed;
}
