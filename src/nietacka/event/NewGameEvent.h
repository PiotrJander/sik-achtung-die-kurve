//
// Created by Piotr Jander on 23/08/2017.
//

#ifndef PROJECT_NEWGAMEEVENT_H
#define PROJECT_NEWGAMEEVENT_H


#include <vector>
#include "GameEvent.h"

class NewGameEvent: public GameEvent {
public:

    /*
     * packed structs ~~~~~~~~~~~~~~~~~~~~~
     */
#pragma pack(push, 1)
    struct DataPacked {
        DataPacked(const NewGameEvent &newGameEvent)
                : maxx(htonl(newGameEvent.getMaxx())),
                  maxy(htonl(newGameEvent.getMaxy()))
        {}

        uint32_t maxx, maxy;
    };

    struct SelfPackedNoPlayerNames
    {
        SelfPackedNoPlayerNames(const NewGameEvent &newGameEvent) : header(newGameEvent), data(newGameEvent)
        {}

        HeaderPacked header;
        DataPacked data;
    };
#pragma pack(pop)
    /*
     * END packed structs ~~~~~~~~~~~~~~~~~~~~~~
     */

    NewGameEvent(uint32_t eventNo, uint32_t maxx, uint32_t maxy);

    NewGameEvent(uint32_t eventNo, uint32_t maxx, uint32_t maxy, std::vector<std::string> playerNames);

    NewGameEvent(const SelfPackedNoPlayerNames &selfPacked, std::vector<std::string> playerNames)
            : GameEvent(selfPacked.header),
              maxx(ntohl(selfPacked.data.maxx)),
              maxy(ntohl(selfPacked.data.maxy)),
              playerNames(playerNames)
    {}

    static std::vector<std::string> parsePlayerNames(const char *buffer, const char *endOfBuffer);

    bool operator==(const GameEvent &other) const override;

    uint32_t getMaxx() const
    {
        return maxx;
    }

    uint32_t getMaxy() const
    {
        return maxy;
    }

    const std::vector<std::string> &getPlayerNames() const
    {
        return playerNames;
    }

    void writeSelf(DynamicBuffer &buffer) override;

private:
    uint32_t maxx, maxy;

    std::vector<std::string> playerNames;

    uint32_t getSizeofPlayerNames() const;

    uint32_t selfLength() const override;
};

#endif //PROJECT_NEWGAMEEVENT_H
