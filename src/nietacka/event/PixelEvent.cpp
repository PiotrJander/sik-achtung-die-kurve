//
// Created by Piotr Jander on 23/08/2017.
//

#include "PixelEvent.h"


void PixelEvent::writeToBuffer(char *buffer)
{
    SelfPacked *buf = reinterpret_cast<SelfPacked *>(buffer);
    *buf = SelfPacked(*this);
}

bool PixelEvent::operator==(const GameEvent &other) const
{
    if (auto *o = dynamic_cast<const PixelEvent *>(&other)) {
        return GameEvent::operator==(other)
               && o->getPlayerNumber() == getPlayerNumber()
               && o->getX() == getX()
               && o->getY() == getY();
    } else {
        return false;
    }
}

void PixelEvent::writeSelf(DynamicBuffer &buffer)
{
    SelfPacked packed(*this);
    buffer << packed;
}


