//
// Created by Piotr Jander on 23/08/2017.
//

#include "GameOverEvent.h"

void GameOverEvent::writeToBuffer(void *buffer)
{
    auto buf = reinterpret_cast<GameOverEvent::HeaderPacked *>(buffer);
    *buf = HeaderPacked(*this);
}
