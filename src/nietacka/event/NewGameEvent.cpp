//
// Created by Piotr Jander on 23/08/2017.
//

#include "NewGameEvent.h"
#include "../easylogging++.h"

NewGameEvent::NewGameEvent(uint32_t eventNo, uint32_t maxx, uint32_t maxy)
        : GameEvent(eventNo, Type::NEW_GAME), maxx(maxx), maxy(maxy), playerNames()
{
    LOG(INFO) << "NewGameEvent";
}

NewGameEvent::NewGameEvent(uint32_t eventNo, uint32_t maxx, uint32_t maxy, std::vector<std::string> playerNames)
        : GameEvent(eventNo, Type::NEW_GAME), maxx(maxx), maxy(maxy), playerNames(playerNames)
{}

uint32_t NewGameEvent::getSizeofPlayerNames() const
{
    uint32_t size = 0;
    for (const std::string &name : playerNames) {
        size += name.size() + 1;
    }
    return size;
}

bool NewGameEvent::operator==(const GameEvent &other) const
{
    if (auto *o = dynamic_cast<const NewGameEvent *>(&other)) {
        return GameEvent::operator==(other)
               && o->getMaxx() == getMaxx()
               && o->getMaxy() == getMaxy();
    } else {
        return false;
    }
}

void NewGameEvent::writeSelf(DynamicBuffer &buffer)
{
    SelfPackedNoPlayerNames packed(*this);
    buffer << packed;

    for (auto &&name : playerNames) {
        buffer.writeCharString(name);
    }
}

uint32_t NewGameEvent::selfLength() const
{
    return sizeof(SelfPackedNoPlayerNames) + getSizeofPlayerNames();
}
