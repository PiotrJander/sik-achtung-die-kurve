//
// Created by Piotr Jander on 23/08/2017.
//

#include "NewGameEvent.h"

NewGameEvent::NewGameEvent(uint32_t eventNo, uint32_t maxx, uint32_t maxy)
        : GameEvent(eventNo, Type::NEW_GAME), data(maxx, maxy), playerNames()
{}

NewGameEvent::NewGameEvent(uint32_t eventNo, uint32_t maxx, uint32_t maxy, std::vector<std::string> playerNames)
        : GameEvent(eventNo, Type::NEW_GAME), data(maxx, maxy), playerNames(), playerNames(playerNames)
{}

NewGameEvent::NewGameEvent(const GameEvent::Header &header, const NewGameEvent::Data &data,
                           std::vector<std::string> playerNames)
        : GameEvent(header), data(data), playerNames(playerNames)
{}

uint32_t NewGameEvent::getLength()
{
    return sizeof(header) + sizeof(data) + getSizeofPlayerNames();
}

std::unique_ptr<char[]> NewGameEvent::getBuffer()
{
    uint32_t length = getLength();
    auto buffer = std::make_unique<char[]>(length);
    std::memcpy(buffer.get(), &header, sizeof(header));
    std::memcpy(buffer.get() + sizeof(header), &data, sizeof(data));
    
    char *writeLocation = buffer.get() + sizeof(header) + sizeof(data);
    for (auto &name : playerNames) {
        strcpy(writeLocation, name.c_str());
        writeLocation += name.size() + 1;
    }
    
    return buffer;
}

uint32_t NewGameEvent::getSizeofPlayerNames()
{
    uint32_t size = 0;
    for (std::string &name : playerNames) {
        size += name.size() + 1;
    }
    return size;
}

std::vector<std::string> NewGameEvent::parsePlayerNames(char *readingLocation, const char *endOfBuffer)
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
