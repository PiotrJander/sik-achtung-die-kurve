//
// Created by Piotr Jander on 23/08/2017.
//

#include "PlayerEliminatedEvent.h"

std::unique_ptr<char[]> PlayerEliminatedEvent::getBuffer()
{
    uint32_t length = getLength();
    auto buffer = std::make_unique<char[]>(length);
    std::memcpy(buffer.get(), &header, sizeof(header));
    std::memcpy(buffer.get() + sizeof(header), &playerNumber, sizeof(playerNumber));
    return buffer;
}
