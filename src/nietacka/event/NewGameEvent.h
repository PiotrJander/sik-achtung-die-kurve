//
// Created by Piotr Jander on 23/08/2017.
//

#ifndef PROJECT_NEWGAMEEVENT_H
#define PROJECT_NEWGAMEEVENT_H


#include <vector>
#include "GameEvent.h"

class NewGameEvent: public GameEvent {
public:
    struct DataPacked;
    struct SelfPackedNoPlayerNames;

    std::vector<std::string> playerNames;

    NewGameEvent(uint32_t eventNo, uint32_t maxx, uint32_t maxy);

    NewGameEvent(uint32_t eventNo, uint32_t maxx, uint32_t maxy, std::vector<std::string> playerNames);

    NewGameEvent(const GameEvent::HeaderPacked &header, const NewGameEvent::DataPacked &data,
                               std::vector<std::string> playerNames);

    static std::vector<std::string> parsePlayerNames(char *buffer, const char *endOfBuffer);

    bool operator==(const GameEvent &other) const override;

    uint32_t getMaxx() const
    {
        return maxx;
    }

    uint32_t getMaxy() const
    {
        return maxy;
    }

private:
    uint32_t maxx, maxy;

    uint32_t getSizeofPlayerNames();

    uint32_t getLength() override;

    void writeToBuffer(void *buffer) override;
};


#pragma pack(push, 1)

struct NewGameEvent::DataPacked {
    DataPacked(const NewGameEvent &newGameEvent)
            : maxx(htonl(newGameEvent.getMaxx())),
              maxy(htonl(newGameEvent.getMaxy()))
    {}

    uint32_t maxx, maxy;
};

struct NewGameEvent::SelfPackedNoPlayerNames
{
    SelfPackedNoPlayerNames(const NewGameEvent &newGameEvent) : header(newGameEvent), data(newGameEvent)
    {}

    GameEvent::HeaderPacked header;
    DataPacked data;
};

#pragma pack(pop)

#endif //PROJECT_NEWGAMEEVENT_H
