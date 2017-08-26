//
// Created by Piotr Jander on 26/08/2017.
//

#include "EventBatch.h"

std::unique_ptr<char[]> EventBatch::getBuffer()
{
    auto buffer = std::make_unique<char[]>(static_cast<size_t>(length));



    return buffer;
}
