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

uint32_t NewGameEvent::selfLength()
{
    return sizeof(SelfPackedNoPlayerNames) + getSizeofPlayerNames();
}

void NewGameEvent::writeToBuffer(char *buffer)
{
    auto buf = reinterpret_cast<SelfPackedNoPlayerNames *>(buffer);
    *buf = SelfPackedNoPlayerNames(*this);

    char *writeLocation = buffer + sizeof(SelfPackedNoPlayerNames);
    for (auto &name : playerNames) {
        strcpy(writeLocation, name.c_str());
        writeLocation += name.size() + 1;
    }
}

uint32_t NewGameEvent::getSizeofPlayerNames()
{
    uint32_t size = 0;
    for (std::string &name : playerNames) {
        size += name.size() + 1;
    }
    return size;
}

std::vector<std::string> NewGameEvent::parsePlayerNames(const char *readingLocation, const char *endOfBuffer)
{
    std::vector<std::string> playerNames;
    std::string string;
    while (readingLocation < endOfBuffer) {
        string = readingLocation;
        readingLocation += string.size() + 1;
        playerNames.emplace_back(std::move(string));
    }
    return playerNames;
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
