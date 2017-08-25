//
// Created by Piotr Jander on 23/08/2017.
//

#include "PixelEvent.h"


std::unique_ptr<char[]> PixelEvent::getBuffer()
{
    uint32_t length = getLength();
    auto buffer = std::make_unique<char[]>(length);
    std::memcpy(buffer.get(), &header, sizeof(header));
    std::memcpy(buffer.get() + sizeof(header), &data, sizeof(data));
    return buffer;
}

bool PixelEvent::operator==(const GameEvent &other) const
{
    if (auto *o = dynamic_cast<const PixelEvent *>(&other)) {
        return GameEvent::operator==(other)
               && o->data.getPlayerNumber() == data.getPlayerNumber()
               && o->data.getX() == data.getX()
               && o->data.getY() == data.getY();
    } else {
        return false;
    }
}


