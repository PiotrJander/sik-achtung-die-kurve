//
// Created by Piotr Jander on 26/08/2017.
//

#include "EventBatch.h"

DynamicBuffer EventBatch::getBuffer()
{
    DynamicBuffer buffer;
    buffer << gameId;

    for (auto &&item : eventHistory) {
        
    }
}
