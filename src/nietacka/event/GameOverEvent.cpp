//
// Created by Piotr Jander on 23/08/2017.
//

#include <memory>
#include "GameOverEvent.h"

std::unique_ptr<char[]> GameOverEvent::getBuffer()
{
    uint32_t length = getLength();
    auto buffer = std::make_unique<char[]>(length);
    std::memcpy(buffer.get(), &header, sizeof(header));
    return buffer;
}
