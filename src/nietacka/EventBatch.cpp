//
// Created by Piotr Jander on 26/08/2017.
//

#include "EventBatch.h"

std::unique_ptr<char[]> EventBatch::getBuffer()
{
    auto bufferBox = std::make_unique<char[]>(static_cast<size_t>(length));
    char *bufferLocation = bufferBox.get();

    for (uint32_t i = startEventNo; i < endEventNo; ++i) {
        bufferLocation += eventHistory.at(i)->writeTo(bufferLocation);
    }

    return bufferBox;
}
