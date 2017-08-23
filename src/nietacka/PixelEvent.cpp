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
    return std::move(buffer);
}

uint32_t PixelEvent::getLength()
{
    return sizeof(header) + sizeof(data);
}
