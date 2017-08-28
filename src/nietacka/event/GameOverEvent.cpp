//
// Created by Piotr Jander on 23/08/2017.
//

#include "GameOverEvent.h"

void GameOverEvent::writeSelf(DynamicBuffer &buffer)
{
    GameOverEvent::HeaderPacked packed(*this);
    buffer << packed;
}
