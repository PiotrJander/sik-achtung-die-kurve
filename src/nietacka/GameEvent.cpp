//
// Created by Piotr Jander on 22/08/2017.
//

#include "GameEvent.h"

void GameEvent::writeTo(std::ostream &s)
{

    s.write((char *) this, sizeof(*this));
}
